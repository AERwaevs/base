#pragma once

#include <concepts>

#include "object.h"
#include "ref_ptr.h"

template< object O = Object >
struct ISingleton : public virtual Object
{
    using type = O;

    static auto instance()
    {
        if( !_instance ) _instance = new O();
        return ref_ptr<O>( _instance );
    }
protected:
    static inline O* _instance;
};

template< typename F, object O >
requires( std::constructible_from<F> )
struct ISingletonFrom : public virtual Object
{
    using From = F;
    
    template< F From, typename T = void, typename std::enable_if_t< std::same_as< T, F > > >
    static auto instance()
    {
        if( !_instance ) _instance = new T();
        return ref_ptr<T>( _instance );
    }
protected:
    static inline O* _instance;
};