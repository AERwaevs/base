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
#include <atomic>
#include <set>

#include "platform.h"
#include "log.h"
#include "object.h"
#include "implements.h"
#include "ref_ptr.h"
#include "spy_ptr.h"
#include "singleton.h"

#define BIT( bit ) 1 << bit

#define AEON_TYPE_NAME( T ) \
    template<> constexpr const char*    AEON::GetName( typename T )    noexcept { return #T; } \
    template<> constexpr const String   AEON::ToString( typename T )   noexcept { return T::GetName(); } \

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
    using Shared        = ref_ptr< Type >;
    template< typename Type >
    using Observer      = spy_ptr< Type >;
    template< typename Type >
    using List          = std::list< Type >;
    template< typename Type1, typename Type2 >
    using Map           = std::map< Type1, Type2 >;
    
    template< typename T, template< typename > typename... _Interfaces >
    struct Interfaces : public virtual _Interfaces<T>...
    {

    };
}