#pragma once

#include <atomic>
#include <typeinfo>
#include <concepts>

#include "platform.h"
#include "interface.h"
#include "ref_ptr.h"

template< typename T >
concept object = requires{ { ref_ptr<T>().get() } -> std::same_as<T*>; };

template< typename T, typename... Args >
concept creatable = requires( Args... args )
{
    { T::create( args... ).get() } -> std::same_as<T*>;
};

class AEON_DLL Object
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

private:
    inline void _ref( std::memory_order order = std::memory_order_relaxed ) const noexcept
    { 
        _references.fetch_add( 1, order );
    }

    inline void _unref( std::memory_order order = std::memory_order_seq_cst ) const noexcept 
    { 
        if( _references.fetch_sub( 1, order ) <= 1 ) delete this;
    }

protected:
    template< class R >
    friend class ref_ptr;

    template< class R >
    friend class Interfaces;
    
private:
    mutable std::atomic_uint32_t _references;
};

template< object O = Object >
struct ICreate
{
    template< typename... Args >
    static constexpr auto create( Args&&... args )
    {
        return ref_ptr<O>( new O( std::forward<Args>( args )... ) );
    }
};

//template< object D, class B = Object, typename... Args  >
//static constexpr auto B::create( Args&&... args )
//{
//    return ref_ptr<D>( new D( std::forward<Args>( args )... ) );
//}

template< object O = Object >
struct ICreateIf
{
    template< typename... Args >
    static constexpr auto create_if( bool flag, Args&&... args )
    {
        return ref_ptr<O>( flag ? new O( std::forward<Args>( args )... ) : nullptr );
    }
};