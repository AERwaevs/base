#pragma once

namespace aer {

template< typename Self, typename T > 
concept accepts = requires ( Self self, T t )
{
    { t.accept( self ) };
};

struct Visitor
{
    template< typename T = void > constexpr
    void apply( T& obj ) {}

    template< typename Self, accepts<Self> T > constexpr
    void apply( this Self&& self, T&& obj ) { self.apply( obj ); }
};

} // namespace aer