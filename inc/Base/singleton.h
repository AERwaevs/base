#pragma once

#include <concepts>

#include "object.h"
#include "ref_ptr.h"

template< object O = Object >
struct ISingleton
{
    using type = O;

    template< typename... Args >
    static auto get_or_create( Args&&... args )
    {
        if( !_singleton ) _singleton = O::create( args... );
        return _singleton;
    }
protected:
    static inline ref_ptr<O> _singleton;
};