#pragma once

#include <atomic>

namespace aer
{

template< typename T >
class ref_ptr;

class Object
{
public:
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
        _references.fetch_add( 1, order );
    }

    inline void _unref( std::memory_order order = std::memory_order_seq_cst ) const noexcept 
    { 
        if( _references.fetch_sub( 1, order ) <= 1 ) delete this;
    }

protected:
    template< typename T >
    friend class ref_ptr;
    
private:
    mutable std::atomic_uint32_t _references;
};

}