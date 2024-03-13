#pragma once

#include "allocator.h"
#include "ref_counter.h"
#include "base_ptr.h"

namespace aer
{


class Object
{
public:
    inline auto ref_count( std::memory_order order = std::memory_order_relaxed ) const noexcept
    { 
        return _references.load(); 
    }
    
    static void* operator new( size_t size )
    {
        return mem::alloc( size );
    }

    static void operator delete( void* ptr )
    {
        mem::dealloc( ptr );
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
    template< typename Ptr >
    requires std::is_pointer_v<Ptr>
    friend struct base_ptr;
    
private:
    mem::ref_counter< uint32_t > _references;
};

}