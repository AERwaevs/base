#pragma once

#include "object.h"

namespace aer {

struct Visitor : public Object
{
    Visitor()                 = default;
    Visitor( const Visitor& ) = default;
    ~Visitor()                = default;

    template< typename Self >
    void visit( this Self&& self, Object& object ) { object.accept( self ); }
};

} // namespace aer