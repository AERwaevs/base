project( aeon-base )

add_library( aeon-base 
    INTERFACE
)

set( precompiled_headers
    <iostream>
    <functional>
    <string>
    <memory>
    <vector>
    <list>
    <chrono>
    <typeinfo>
    <map>
    <atomic>
    <set>

    "platform.h"
    "log.h"
    "object.h"
    "implements.h"
    "ref_ptr.h"
    "spy_ptr.h"
    "singleton.h"
)

set_target_properties( aeon-base
    PROPERTIES
    LINKER_LANGUAGE             CXX
    PUBLIC_HEADER               "Base.h"
    POSITION_INDEPENDENT_CODE   TRUE
)

target_precompile_headers( aeon-base INTERFACE ${precompiled_headers} )

install( TARGETS aeon-base )