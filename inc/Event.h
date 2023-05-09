#pragma once

#include "Base.h"

namespace AEON
{
    struct Event : public Object
    {
        virtual ~Event() = default;

        bool handled() const { return _handled; }

        template< typename E, typename T, typename F > 
        bool Dispatch( const T& target, const F& function )
        {
            if( typeid( *this ) != typeid( E ) ) return false;

            _handled |= (target->*function)( static_cast<E&>(*this) );
            AE_WARN_IF( !_handled, "Unhandled %s", type_name<E>() );
            return _handled;
        }

        //* Attempt #384293 at using concepts to constrain template deduction.
        //template< typename E, typename F >
        //requires std::invocable< F, E& >
        //bool Dispatch( const F& function )
        //{
        //    if( typeid( *this ) != typeid( E ) ) return false;

        //    _handled |= function( static_cast<E&>(*this) );
        //    AE_WARN_IF( !_handled, "Unhandled %s", type_name( E ) );
        //    return _handled;
        //}

    private:
        bool      _handled = false;
    };

    using Events = List<ref_ptr<Event>>;
}