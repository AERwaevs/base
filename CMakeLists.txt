# cmake -------------------------------------------------------------------------------------------
cmake_minimum_required( VERSION 3.2.0 )
# AEON::Base --------------------------------------------------------------------------------------
set( BASE_LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib )
set( BASE_INC ${CMAKE_CURRENT_SOURCE_DIR}/inc )
set( BASE_SRC ${BASE_INC}/Base )

set( STD_HEADERS
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
)

set( HEADERS
    ${BASE_SRC}/Base.h
    ${BASE_SRC}/platform.h
    ${BASE_SRC}/log.h
    ${BASE_SRC}/object.h
    ${BASE_SRC}/implements.h
    ${BASE_SRC}/ref_ptr.h
    ${BASE_SRC}/spy_ptr.h
    ${BASE_SRC}/singleton.h
)

set( LOGURU_SRC 
    ${BASE_LIB}/loguru/loguru.hpp 
    ${BASE_LIB}/loguru/loguru.cpp
)

add_library( base STATIC ${HEADERS} ${LOGURU_SRC} )
set_target_properties( base
    PROPERTIES 
    CXX_STANDARD    20
    LINKER_LANGUAGE CXX
)
target_include_directories( base 
    PUBLIC
        $<BUILD_INTERFACE:${BASE_INC}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        $<BUILD_INTERFACE:${BASE_LIB}>
)
#target_precompile_headers( base PUBLIC ${STD_HEADERS} )
target_compile_features(   base PUBLIC cxx_std_20 )
target_compile_definitions( base PUBLIC AEON_DEBUG )

add_library( aer::base ALIAS base )