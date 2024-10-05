#pragma once

#include <utility>
#include <memory>

#include "ref_counter.h"

namespace aer { namespace mem {

template< typename T >
struct managed_object
{
    inline auto ref_count( std::memory_order order = std::memory_order_seq_cst ) const noexcept
    { 
        return _references.load( order ); 
    }
    inline void ref( std::memory_order order = std::memory_order_seq_cst ) const noexcept
    { 
        _references.increment( order );
    }
    inline void unref( std::memory_order order = std::memory_order_seq_cst ) const noexcept 
    { 
        if( _references.decrement( order ) ) delete this;
    }
protected:
    template< typename... Args > requires( std::constructible_from<T, Args...> )
    explicit managed_object( Args&&... args ) noexcept : _references( 0 )
    {
        new( &_storage ) T( std::forward<Args>( args )... );
    }
    virtual ~managed_object() noexcept               = default;
             managed_object( const managed_object& ) = delete;
             managed_object( managed_object&& )      = delete;
protected:
          T*    get()       noexcept { return std::launder( reinterpret_cast<T*>( &_storage ) ); }
    const T*    get() const noexcept { return std::launder( reinterpret_cast<const T*>( &_storage ) ); }
    const void  dispose()   noexcept { get()->~T(); }
private:
    mutable ref_counter< uint32_t > _references{ 0 };
    alignas(alignof(T))  uint8_t    _storage[ sizeof(T) ];
};

}} // namespace aer::mem