#pragma once

#include "object.h"
#include "memory/ref_ptr.h"

namespace aer
{
    
template< typename T, typename... Args > requires( std::constructible_from<T, Args...> ) static inline
auto create( Args&&... args )
{
    return ref_ptr<T>( new T( std::forward<Args>( args )... ) );
}

} // namespace aer
