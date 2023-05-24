#pragma once

#include "Event.h"
#include "Base.h"

namespace AEON
{

template< typename T >
struct IEventListener
{
    template< typename E >
    requires std::derived_from< E, Event > || std::common_reference_with< E, Event >
    inline  void SendEvent( E&& event ) { _events.emplace_back( event ); };

    virtual bool PollEvents( Events& poll_to, bool clear_unhandled = true )
    {
        if( _events.empty() ) return false;

        _events.remove_if( []( const auto& e ){ return e->handled(); } );
        poll_to.splice( poll_to.end(), _events );
        if( clear_unhandled ) _events.clear();
        return _events.empty();
    };
protected:
    template< typename E, typename F >
    requires std::invocable< F, E >
    bool OnEvent( E& event ) 
    {
        return F( event );
    }
    
protected:
    Events _events;
};

} // namespace AEON