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
// counter is 2^(8*sizeof(T)-2) - 1

template< std::unsigned_integral T = uint32_t > requires( !std::same_as<T, bool> )
struct ref_counter
{
             ref_counter()            noexcept : _count( 1u ) {}
    explicit ref_counter( T desired ) noexcept : _count( desired ) {}
            ~ref_counter()            noexcept = default;

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
            T expected = 0;
            if( _count.compare_exchange_strong( expected, ZERO_FLAG ) ) return true;
            else if( ( expected & ZERO_PENDING_FLAG ) && ( _count.exchange( ZERO_FLAG ) & ZERO_PENDING_FLAG ) ) return true;
        }
        return false;
    }

    // Loads the current value of the counter.
    // If the current value is zero, it is guaranteed to remain zero
    T load( std::memory_order order = std::memory_order_seq_cst ) const noexcept
    {
        auto val = _count.load( order );
        if( val == 0 && _count.compare_exchange_strong( val, ZERO_FLAG | ZERO_PENDING_FLAG ) ) return 0;
        return ( val & ZERO_FLAG ) ? 0 : val;
    }

    explicit operator T()   const noexcept { return load(); }
        bool operator ++ () const noexcept { return increment(); }
        bool operator -- () const noexcept { return decrement(); }

private:
    enum : T
    {
        ZERO_FLAG           = ( 1u << ( sizeof(T) * 8u - 1u ) ),
        ZERO_PENDING_FLAG   = ( 1u << ( sizeof(T) * 8u - 2u ) )
    };

#if ( false ) // raw memory view for debugging purposes
    mutable struct
    {
        int     value : sizeof(T) * 8u - 2u = 1;
        bool    zero  : 1 = 0;
        bool    pending_zero : 1 = 0;
    };
    struct
    {
        mutable std::atomic<T> _count = 1;
    };
#else
    mutable std::atomic<T> _count;
#endif
};

} // namespace aer::mem
