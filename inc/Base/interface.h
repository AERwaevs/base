#pragma once

namespace aer
{

template< typename T, template< typename > typename... interfaces >
struct Interfaces : public virtual interfaces<T>...
{

};

template< typename T, template< typename > typename I >
concept has_interface = std::derived_from< T, I<T> >;

}