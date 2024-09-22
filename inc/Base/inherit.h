#pragma once

#include "object.h"
#include "memory/ref_ptr.h"

namespace aer
{
    
template< typename derived_t, typename base_t >
requires( std::same_as< base_t, Object > || std::derived_from< base_t, Object > )
struct inherit : public base_t
{
    using Base = base_t;

    //! constraining this makes the compiler argue that classes are NOT nothrow_destructible
    template< typename... Args > /*requires( std::constructible_from<derived_t, Args...> )*/ static inline
    auto create( Args&&... args )
    {
        return ref_ptr<derived_t>( new derived_t( std::forward<Args>( args )... ) );
    }

    template< typename Self > constexpr
    bool is_compatible( this Self&&, const std::type_info& type ) noexcept
    {
        return typeid( Self ) == type || base_t::is_compatible( type );
    }

protected:
    template< typename... Args >
    inherit( Args&&... args ) : base_t( std::forward<Args>(args)... ) {};

    virtual ~inherit() noexcept = default;
};

} // namespace aer
