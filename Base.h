#pragma once

#include <iostream>
#include <functional>
#include <string>
#include <memory>
#include <vector>
#include <list>
#include <chrono>
#include <typeinfo>
#include <map>
#include <set>

#include "platform.h"
#include "log.h"
#include "object.h"
#include "implements.h"
#include "ref_ptr.h"
#include "spy_ptr.h"
#include "singleton.h"

#define BIT( bit ) 1 << bit
#define STR( str ) #str

#define AEON_TYPE_NAME( T ) \
    template<> constexpr const char* type_name<T>()                 noexcept { return #T; } \
    template<> constexpr const char* type_name<const T>( const T& ) noexcept { return "const "#T; } \

namespace AEON
{
    using floats            = std::vector<float>;
    using ints              = std::vector<int>;
    using String            = std::string;
    using SteadyClock       = std::chrono::steady_clock;
    using TimePoint         = std::chrono::steady_clock::time_point;
    using HighResClock      = std::chrono::high_resolution_clock;
    using HighResTimePoint  = std::chrono::high_resolution_clock::time_point;

    template< typename Type >
    using Function      = std::function< Type >;
    template< typename Type >
    using Vector        = std::vector< Type >;
    template< typename Type >
    using List          = std::list< Type >;
    template< typename Type1, typename Type2 >
    using Map           = std::map< Type1, Type2 >;

    template< typename T > constexpr const char* type_name()           noexcept { return typeid(T).name(); }
    template< typename T > constexpr const char* type_name( const T& ) noexcept { return type_name<T>(); }

    template< typename T >
    struct ITypeInfo
    {
        constexpr const char*           type_name() noexcept { return type_name<T>(); }
        constexpr const std::type_info& type_info() noexcept { return typeid(T);      }
        constexpr const std::size_t     type_size() noexcept { return sizeof(T);      }
    };

}