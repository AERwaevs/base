#pragma once

#include <utility>

namespace aer
{

template< typename T >
struct ICreate
{
    template< typename... Args >
    static constexpr auto create( Args&&... args )
    {
        return ref_ptr<T>( new T( std::forward<Args>( args )... ) );
    }
};

template< typename T >
struct ICreateIf
{
    template< typename... Args >
    static constexpr auto create_if( bool flag, Args&&... args )
    {
        return ref_ptr<T>( flag ? new T( std::forward<Args>( args )... ) : nullptr );
    }
};

} // namespace aer
