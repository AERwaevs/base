#pragma once

#include <concepts>

#include "memory/Allocator.h"
#include "memory/ref_ptr.h"

#include "type_name.h"
#include "log.h"

namespace aer
{
    
class Object
{
public:
    static void* operator new( size_t size )  { return mem::alloc( size ); }
    static void  operator delete( void* ptr ) { mem::dealloc( ptr ); }

    template< typename Self > constexpr
    auto& type_info( this Self&& ) noexcept { return typeid( Self ); }

    template< typename Self > constexpr
    auto type_size( this Self&& ) noexcept { return sizeof( Self ); }

    template< typename Self > constexpr
    auto type_name( this Self&& ) noexcept { return aer::type_name< Self >(); }

    template< typename Self > constexpr
    bool is_compatible( this Self&&, const std::type_info& type ) noexcept { return typeid( Self ) == type; }

    template< typename Self, typename V > constexpr
    void accept( this Self&& self, V& visitor ) { visitor.visit( self ); }

    inline auto ref_count( std::memory_order order = std::memory_order_relaxed ) const noexcept
    { 
        return _references.load(); 
    }
protected:
             Object() noexcept = default;
    virtual ~Object() noexcept       = default;
             Object( Object&& )      = delete;
             Object( const Object& ) = delete;

    inline void _ref( std::memory_order order = std::memory_order_relaxed ) const noexcept
    { 
        _references++;
    }

    inline void _unref( std::memory_order order = std::memory_order_seq_cst ) const noexcept 
    { 
        if( _references-- == 0 ) delete this;
    }

protected:
    template< typename T >
    friend class ref_ptr;
    
private:
    mutable std::atomic_uint32_t _references;
};

template< std::derived_from<Object> T, typename... Args > requires( std::constructible_from<T, Args...> )
constexpr inline ref_ptr<T> create( Args&&... args ) noexcept
{
    return ref_ptr<T>{ new T(std::forward<Args>( args )...) };
};

} // namespace aer