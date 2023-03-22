#pragma once

#include "object.h"
#include "ref_ptr.h"

template< typename T >
class Singleton : public object_t<>
{
public:
    template< typename... Args >
    static ref_ptr<T>& instance( Args&&... args )
    {
        static ref_ptr<T> s_instance( new T( std::forward<Args>( args )... ) );
        return s_instance;
    }
protected:
    template< typename... Args >
    Singleton( Args&&... args ) : object_t( std::forward<Args>( args )... ) {}
};