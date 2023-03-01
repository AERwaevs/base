project( aeon-base )

add_library( aeon-base 
    STATIC
    Base.cpp
)
set_target_properties( aeon-base
    PROPERTIES
    LINKER_LANGUAGE             CXX
    PUBLIC_HEADER               "Base.h"
    POSITION_INDEPENDENT_CODE   TRUE
)
target_include_directories( aeon-base PUBLIC        ${CMAKE_CURRENT_SOURCE_DIR} )
target_precompile_headers(  aeon-base PUBLIC        ${AEON_PRECOMP}     )

install( TARGETS aeon-base )
