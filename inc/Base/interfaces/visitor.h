#pragma once

#include <concepts>

namespace aer
{

template< typename T >
struct IVisitor
{

    template< typename R >
    virtual void apply( R& )

}

} // namespace aer
