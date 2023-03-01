#pragma once

#include <atomic>
#include <typeinfo>

#include "platform.h"
#include "ref_ptr.h"

template< std::unsigned_integral ref_ctr = uint32_t >
class AEON_DLL object_t
{
    static constexpr inline auto default_order = std::memory_order_seq_cst;

public:
    inline auto ref_count( std::memory_order order = default_order ) const noexcept
    { 
        return _references.load(); 
    }

protected:
    explicit object_t() noexcept : _references( 0 ) {}
    virtual ~object_t()                             {}
             object_t( object_t&& )      = delete;
             object_t( const object_t& ) = delete;

private:
    inline auto ref( std::memory_order order = default_order ) const noexcept
    { 
        _references.fetch_add( 1, order );
    }

    inline auto unref( std::memory_order order = default_order ) const noexcept 
    { 
        if( _references.fetch_sub( 1, order ) <= 1 ) delete this;
    }

protected:
    template< class R >
    friend class ref_ptr;

private:
    mutable std::atomic<ref_ctr> _references;
};

template< typename T >
class AEON_DLL Object : public object_t<>
{
public:
    template< typename... Args >
    static inline auto create( Args&&... args )
    {
        return ref_ptr<T>( new T( std::forward<Args>( args )... ) );
    }
protected:
    template< typename... Args >
    Object( Args&&... args ) : object_t() {}
};