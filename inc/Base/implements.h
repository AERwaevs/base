#pragma once

#include <concepts>

#include "object.h"

template< object T, template< typename > typename... Interfaces >
struct Implements : public virtual Interfaces<T>...
{

};