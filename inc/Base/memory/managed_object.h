#pragma once

#include <utility>
#include <memory>
#include <concepts>

#include "ref_counter.h"

namespace aer {
namespace mem {

enum class eject_action : uint8_t
{
    nothing,
    defer,
    destroy
};

template< std::destructible T >
struct managed_object
{
    using type = T;

    void* operator new( size_t size )  { return mem::alloc( size ); }
    void  operator delete( void* ptr ) { mem::dealloc( ptr ); }

    template< typename... Args > requires( std::constructible_from<T, Args...> )
    explicit managed_object( Args&&... args ) noexcept { new( &_storage ) type( std::forward<Args>( args )... ); }
             managed_object( type&& new_obj ) noexcept { new( &_storage ) type( std::move( new_obj ) ); }
             managed_object( type* ptr )      noexcept { new( &_storage ) type( std::move( *ptr ) ); }
             managed_object( const managed_object& ) = default;
             managed_object( managed_object&& )      = default;
            ~managed_object() noexcept               = default;

          type*  get()              noexcept { return std::launder( reinterpret_cast<T*>( &_storage ) ); }
    const type*  get()        const noexcept { return std::launder( reinterpret_cast<const T*>( &_storage ) ); }
    auto         weak_refs()  const noexcept { return _weak_refs.load(); }
    auto         hard_refs()  const noexcept { return _hard_refs.load(); }
    bool         ref_weak()   const noexcept { return _weak_refs.increment( std::memory_order_relaxed ); }
    bool         ref_hard()   const noexcept { return _hard_refs.increment( std::memory_order_relaxed ); }
    bool         unref_weak() const noexcept { return _weak_refs.decrement( std::memory_order_release ) == 0; }
    eject_action unref_hard() const noexcept 
    { 
        if( _hard_refs.decrement( std::memory_order_release ) == false ) return eject_action::nothing;
        std::atomic_thread_fence( std::memory_order_acquire );
        if( _weak_refs.load( std::memory_order_relaxed ) != 0 ) return eject_action::defer;
        else dispose(); return eject_action::destroy;
        
    }

protected:
    const void dispose() const noexcept { get()->~type(); }
private:
    alignas(alignof(T)) std::byte _storage[sizeof(T)];
    ref32_t _weak_refs{ 0 };
    ref32_t _hard_refs{ 0 };
};

}} // namespace aer::mem