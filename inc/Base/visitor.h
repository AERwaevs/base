#pragma once

#include "object.h"

namespace aer
{

class Visitor : public Object
{
public:
    Visitor() = default;

    template< typname Self, typename T >
    requires std::same_as< T, Object> || std::derived_from< T, Object >
    void apply( this Self&, T& );
};

} // namespace aer