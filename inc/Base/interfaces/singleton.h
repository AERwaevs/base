#pragma once

#include <concepts>

#include "../ref_ptr.h"

template< typename T >
struct ISingleton
{
    using type = T;

    template< typename... Args >
    static auto get_or_create( Args&&... args )
    {
        if( !_singleton ) _singleton = new T( std::forward<Args>( args )... );
                          _singleton->_ref();
        return            _singleton;
    }
protected:
    static inline T* _singleton;
};