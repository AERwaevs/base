#pragma once

namespace aer
{

template< typename T > constexpr const char* type_name() noexcept
{
#ifdef CXX_DEMANGLE
    int    status( -1 );
    auto   name = typeid(T).name();
    auto   demangled = abi::__cxa_demangle( name, 0, 0, &status );
    return status == 0 ? demangled : name;
#else
    return typeid(T).name();
#endif
}
template< typename T > constexpr const char* type_name( const T& ) noexcept { return type_name<T>(); }

template< typename T >
struct ITypeInfo
{   
    static constexpr const char*           type_name() noexcept { return aer::type_name<T>(); }
    static constexpr const std::type_info& type_info() noexcept { return typeid(T);           }
    static constexpr const std::size_t     type_size() noexcept { return sizeof(T);           }
};


}