#pragma once

#include "memory/Allocator.h"
#include "memory/ref_counter.h"
#include "memory/ref_ptr.h"

#include "interfaces/type_info.h"

namespace aer
{

struct Object
{    
    static void* operator new( size_t size )    noexcept { return mem::alloc( size ); }
    static void  operator delete( void* ptr )   noexcept { mem::dealloc( ptr ); }

    template< typename T, typename... Args > static constexpr
    auto create( Args&&... args ) noexcept
    {
        return ref_ptr<T>::create( std::forward<Args>( args )... );
    }

    template< typename T, typename... Args > static constexpr
    auto create_if( bool flag, Args&&... args ) noexcept
    {
        return flag ? ref_ptr<T>::create( std::forward<Args>( args )... ) : ref_ptr<T>();
    }

    template< typename Self > constexpr
    auto& type_info( this Self&& ) noexcept { return typeid( Self ); }

    template< typename Self > constexpr
    auto type_size( this Self&& ) noexcept { return sizeof( Self ); }

    template< typename Self > constexpr
    auto type_name( this Self&& ) noexcept { return aer::type_name< Self >(); }

    template< typename Self > constexpr
    bool is_compatible( this Self&&, const std::type_info& type ) noexcept { return typeid( Self ) == type; }

    inline auto ref_count( std::memory_order order = std::memory_order_relaxed ) const noexcept
    { 
        return _references.load(); 
    }
protected:
    explicit Object() noexcept : _references( 0 ) {}
    virtual ~Object()                = default;
             Object( Object&& )      = delete;
             Object( const Object& ) = delete;

    inline void _ref( std::memory_order order = std::memory_order_relaxed ) const noexcept
    { 
        _references.increment();
    }

    inline void _unref( std::memory_order order = std::memory_order_seq_cst ) const noexcept 
    { 
        if( _references.decrement() ) delete this;
    }

protected:
    template< typename >
    friend class ref_ptr;
    
private:
    mutable mem::ref_counter< uint32_t > _references;
};

template< typename T, typename... Args >
requires std::constructible_from< T, Args... > && std::derived_from< T, Object >
static constexpr ref_ptr<T> create( Args&&... args ) { return new T( std::forward<Args>( args )... ); }

} // namespace aer