#pragma once

#include "object.h"

namespace aer
{

template< typename Derived, typename Base >
struct Inherit : Base
{
    using Base::Base;
    
    template< typename... Args > static constexpr
    auto create( Args&&... args ) noexcept
    {
        return ref_ptr<Derived>::create( std::forward<Args>( args )... );
    }

    template< typename... Args > static constexpr
    auto create_if( bool flag, Args&&... args ) noexcept
    {
        return flag ? ref_ptr<Derived>::create( std::forward<Args>( args )... ) : ref_ptr<Derived>();
    }

    template< typename Self > constexpr
    bool is_compatible( this Self&& self, const std::type_info& type ) noexcept
    {
        return typeid( Self ) == type || static_cast<Base&&>( self ).is_compatible( type );
    }

protected:
    template< typename... Args >
    Inherit( Args&&... args ) : Base( std::forward<Args>( args )... ) {}
};

} // namespace aer