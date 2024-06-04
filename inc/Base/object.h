#pragma once

#include "memory/Allocator.h"
#include "memory/ref_counter.h"

namespace aer
{


class Object
{
public:    
    static void* operator new( size_t size )    noexcept { return mem::alloc( size, mem::ALLOCATOR_AFFINITY_OBJECTS ); }
    static void  operator delete( void* ptr )   noexcept { mem::dealloc( ptr ); }

    template< typename Self > constexpr
    auto accept( this Self& self, Visitor& visitor ) noexcept { return visitor.apply( self ); }

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