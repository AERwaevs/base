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

#include <Base/platform>
#include <Base/inherit>
#include <Base/object>
#include <Base/ref_ptr>
#include <Base/observer_ptr>

#include <Core/Log.h>

#define BETTER_ENUMS_MACRO_FILE <better-enums/enum_256.h>
#include <better-enums/enum.h>

#define BIT( bit ) 1 << bit

#define BIND_WITH_ARGS( func ) \
    [this]( auto&&... args ) -> decltype( auto ) \
    { \
        return this->func( std::forward< decltype( args ) >( args )... ); \
    }

#define AEON_TYPE_NAME( T ) \
    template<> constexpr const char*    AEON::GetName( typename T )    noexcept { return #T; } \
    template<> constexpr const String   AEON::ToString( typename T )   noexcept { return T::GetName(); } \

namespace AEON
{
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
    using Observer      = observer_ptr< Type >;
    template< typename Type >
    using List          = std::list< Type >;
    template< typename Type1, typename Type2 >
    using Map           = std::map< Type1, Type2 >;
}