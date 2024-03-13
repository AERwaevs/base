#pragma once

#include <utility>

#include "../memory/ptr_ref.h"
namespace aer
{

template< typename T >
struct ISingleton
{
    template< typename... Args >
    static auto get_or_create( Args&&... args )
    {
        if( !_singleton ) _singleton = T::create( std::forward<Args>( args )... );
        return _singleton;
    }
protected:
    static inline ref_ptr<T> _singleton;
};

} // namespace aer