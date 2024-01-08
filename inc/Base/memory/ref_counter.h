#pragma once

#include <stdint.h>
#include <atomic>

namespace aer::mem
{

// A wait-free atomic counter that supports increment and decrement,
// such that attempting to increment the counter from zero fails and
// does not perform the increment.
//
// Useful for implementing reference counting, where the underlying
// managed memory is freed when the counter hits zero, so that other
// racing threads can not increment the counter back up from zero
//
// Assumption: The counter should never go negative. That is, the
// user should never decrement the counter by an amount greater
// than its current value
//
// Note: The counter steals the top two bits of the integer for book-
// keeping purposes. Hence the maximum representable value in the
// counter is 2^(8*sizeof(uint32_t)-2) - 1
struct ref_counter
{
             ref_counter()                   noexcept : _count( 1u ) {}
    explicit ref_counter( uint32_t desired ) noexcept : _count( desired == 0 ? ZERO_FLAG : desired ) {}

    // Increment if the counter is not zero.
    //
    // Returns true if the increment was successful, i.e., the counter
    // was not stuck at zero. Returns false if the counter was zero
    bool increment( std::memory_order order = std::memory_order_seq_cst ) const noexcept
    {
        return ( _count.fetch_add( 1u, order ) & ZERO_FLAG ) == 0;
    }

    // Decrement the counter, it is assumed this will never be decremented below zero
    //
    // Returns true if the counter was decremented to zero
    // Returns false if the counter was not decremented to zero
    bool decrement( std::memory_order order = std::memory_order_seq_cst ) const noexcept
    {
        if( _count.fetch_sub( 1u, order ) == 1u )
        {
            uint32_t expected = 0;
            if( _count.compare_exchange_strong( expected, ZERO_FLAG ) ) return true;
            else if( ( expected & ZERO_PENDING_FLAG ) && ( _count.exchange( ZERO_FLAG ) & ZERO_PENDING_FLAG ) ) return true;
        }
        return false;
    }

    // Loads the current value of the counter.
    // If the current value is zero, it is guaranteed to remain zero
    uint32_t load( std::memory_order order = std::memory_order_seq_cst ) const noexcept
    {
        auto val = _count.load( order );
        if( val == 0 && _count.compare_exchange_strong( val, ZERO_FLAG | ZERO_PENDING_FLAG ) ) return 0;
        return ( val & ZERO_FLAG ) ? 0 : val;
    }

private:
    enum : uint32_t
    {
        ZERO_FLAG           = ( 1u << ( sizeof(uint32_t) * 8u - 1u ) ),
        ZERO_PENDING_FLAG   = ( 1u << ( sizeof(uint32_t) * 8u - 2u ) )
    };

    mutable std::atomic_uint32_t _count;
};

} // namespace aer::mem
