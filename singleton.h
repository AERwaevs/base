#pragma once

#include "object.h"
#include "ref_ptr.h"

template< object T = ICreatable<> >
struct ISingleton
{
    template< typename... Args >
    static ref_ptr<T> instance( Args&&... args )
    {
        if( !_instance ) _instance = new T( std::forward<Args>( args )... );
        return ref_ptr<T>( _instance );
    }

    template< typename Type, typename... Args >
    requires std::derived_from< Type, T >
    static auto instance( Args&&... args )
    {
        return _cast<Type>( instance() );
    }
protected:
    static inline T* _instance;
};