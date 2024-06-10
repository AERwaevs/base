#pragma once

namespace aer
{

template< typename T > constexpr auto type_name()            noexcept { return typeid(T).name(); }
template< typename T > constexpr auto type_name( T& )        noexcept { return type_name<T>(); }
template< typename T > constexpr auto type_name( const T& )  noexcept { return type_name<const T>(); }     

#define AER_TYPE_NAME( T )\
template<> auto type_name<T>()         noexcept { return #T; }\
template<> auto type_name<const T>()   noexcept { return "const "#T; }

}