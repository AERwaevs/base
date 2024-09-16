#pragma once

#include "Base.h"

namespace aer
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
            AE_WARN_IF( !_handled, "Unhandled %s", type_name() );
            return _handled;
        }

    private:
        bool      _handled = false;
    };

    using Events = std::list<ref_ptr<Event>>;
}