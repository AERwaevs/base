#pragma once

#include <utility>
#include <memory>

#include "ref_counter.h"

namespace aer {
namespace mem {

enum class eject_action : uint8_t
{
    nothing,
    delay,
    destroy
};

template< std::destructible T >
struct managed_object
{
    inline auto ref_count( std::memory_order order = std::memory_order_seq_cst ) const noexcept
    { 
        return _hard_refs.load( order ); 
    }
    inline bool hard_ref( std::memory_order order = std::memory_order_relaxed ) const noexcept
    { 
        _hard_refs.increment( order );
    }
    inline bool weak_ref( std::memory_order order = std::memory_order_relaxed ) const noexcept
    { 
        _weak_refs.increment( order );
    }
    inline eject_action unref( std::memory_order order = std::memory_order_release ) const noexcept 
    { 
        if( _hard_refs.decrement( order ) )
        {
            std::atomic_thread_fence( std::memory_order_acquire );
            if( _weak_refs.load() <= 1 ) { dispose(); return eject_action::destroy; }
            else return eject_action::delay;
        }
        return eject_action::nothing;
    }
    inline bool weak_unref( std::memory_order order = std::memory_order_release ) const noexcept 
    { 
        return _weak_refs.decrement( order ) == 0;
    }
protected:
    template< typename... Args > requires( std::constructible_from<T, Args...> )
    explicit managed_object( Args&&... args ) noexcept : _hard_refs( 0 ), _weak_refs( 0 )
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
    alignas(alignof(T))  uint8_t    _storage[ sizeof(T) ];
                         ref32_t    _hard_refs;
                         ref32_t    _weak_refs;
};

}} // namespace aer::mem