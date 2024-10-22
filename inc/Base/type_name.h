#pragma once

namespace aer
{

template< typename T > constexpr auto type_name()            noexcept { return typeid(T).name(); }
template< typename T > constexpr auto type_name( const T& )  noexcept { return type_name<T>(); }

}

#define TYPE_NAME( T )\
template<> constexpr auto type_name<T>()       noexcept { return #T; }\
template<> constexpr auto type_name<const T>() noexcept { return "const "#T; }

#define EXT_TYPE_NAME( T )\
template<> constexpr auto aer::type_name<T>()         noexcept { return #T; }\
template<> constexpr auto aer::type_name<const T>()   noexcept { return "const "#T; }