#pragma once

#include <typeinfo>

#include "type_name.h"
#include "visitor.h"

namespace aer {

struct Object
{
    constexpr auto& type_info(  this auto&& self ) noexcept { return typeid( self ); }
    constexpr auto  type_size(  this auto&& self ) noexcept { return sizeof( self ); }
    constexpr auto  type_name(  this auto&& self ) noexcept { return aer::type_name( self ); }

    template< std::derived_from<Visitor> V > constexpr
    void accept( this auto&& self, V& visitor ) noexcept { visitor.apply( self ); }
};
TYPE_NAME( aer::Object );

} // namespace aer

