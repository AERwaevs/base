#pragma once

#include <typeinfo>

#include "memory/Allocator.h"
#include "memory/ref_counter.h"
#include "memory/managed_object.h"
#include "type_name.h"
#include "visitor.h"

namespace aer {

struct Object
{
    void* operator new( size_t size )  { return mem::alloc( size ); }
    void  operator delete( void* ptr ) { mem::dealloc( ptr ); }

    constexpr auto& type_info( this auto&& self ) noexcept { return typeid( self ); }
    constexpr auto  type_size( this auto&& self ) noexcept { return sizeof( self ); }
    constexpr auto  type_name( this auto&& self ) noexcept { return aer::type_name( self ); }

    template< typename Self, typename T >
    constexpr auto  compatible( this Self&&, const T& ) noexcept { return typeid(Self) == typeid(T) || std::derived_from<T, Self>; }
    template< typename Self >
    constexpr auto  compatible( this Self&& self, const std::type_info& type ) noexcept { return typeid(Self) == type || self.compatible( type ); }

    explicit Object()                noexcept = default;
             Object( const Object& ) noexcept = delete;
             Object( Object&& )      noexcept = delete;
            ~Object() noexcept                = default;

    auto              weak_refs()  const noexcept { return _weak_refs.load(); }
    auto              hard_refs()  const noexcept { return _hard_refs.load(); }
    bool              ref_weak()   const noexcept { return _weak_refs.increment( std::memory_order_relaxed ); }
    bool              ref_hard()   const noexcept { return _hard_refs.increment( std::memory_order_relaxed ); }
    bool              unref_weak() const noexcept { return _weak_refs.decrement( std::memory_order_release ) == 0; }
    mem::eject_action unref_hard() const noexcept 
    { 
        if( _hard_refs.decrement( std::memory_order_release ) == false ) return mem::eject_leave;
        std::atomic_thread_fence( std::memory_order_acquire );
        if( _weak_refs.load( std::memory_order_relaxed ) != 0 ) return mem::eject_defer;
        else delete this; return mem::eject_destroy;
    }

    template< std::derived_from<Visitor> V > constexpr
    void accept( this auto&& self, V& visitor ) noexcept { visitor.apply( self ); }

private:
    mem::ref32_t _weak_refs{ 0 };
    mem::ref32_t _hard_refs{ 0 };
};
TYPE_NAME( aer::Object );

} // namespace aer

