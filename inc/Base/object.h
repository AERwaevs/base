#pragma once

#include "memory/Allocator.h"
#include "memory/ref_counter.h"

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
    template< typename T >
    friend class ref_ptr;
    
private:
    mutable mem::ref_counter< uint32_t > _references;
};

} // namespace aer