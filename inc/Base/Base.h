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
#include <concepts>
#include <atomic>
#include <set>
#include <fstream>

#include "platform.h"
#include "log.h"
#include "object.h"
#include "ref_ptr.h"
#include "spy_ptr.h"
#include "singleton.h"
#include "interface.h"
#include "io/read.h"

#define BIT( bit ) 1 << bit
#define STR( str ) #str

#define AEON_TYPE_NAME( T ) \
    template<> constexpr const char* type_name<T>()                 noexcept { return #T; } \
    template<> constexpr const char* type_name<const T>( const T& ) noexcept { return "const "#T; } \

namespace aer
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
        constexpr const char*           type_name( const T& ) noexcept { return typeid(*this).name(); }
        constexpr const std::type_info& type_info( const T& ) noexcept { return typeid(*this);        }
        constexpr const std::size_t     type_size( const T& ) noexcept { return sizeof(*this);        }

        static constexpr const char*           type_name() noexcept { return typeid(T).name(); }
        static constexpr const std::type_info& type_info() noexcept { return typeid(T);        }
        static constexpr const std::size_t     type_size() noexcept { return sizeof(T);        }
    };

}