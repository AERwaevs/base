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

add_library( aeon-base STATIC ${HEADERS} )
set_target_properties( aeon-base
    PROPERTIES 
    CXX_STANDARD    20
    LINKER_LANGUAGE CXX
)
target_include_directories( aeon-base 
    PUBLIC
        $<BUILD_INTERFACE:${BASE_INC}>
        $<BUILD_INTERFACE:${BASE_LIB}>
        $<INSTALL_INTERFACE:inc>
)
#target_precompile_headers( aeon-base PUBLIC ${STD_HEADERS} )
target_compile_features(   aeon-base PUBLIC cxx_std_20 )
target_compile_definitions( aeon-base PUBLIC AEON_DEBUG )
# submodules --------------------------------------------------------------------------------------
find_package( Git QUIET )
if( GIT_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git" )
    option( GIT_SUBMODULE "Check submodules during build" ON )
    if( GIT_SUBMODULE )
        message(STATUS "Base submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()
# loguru ------------------------------------------------------------------------------------------
# loguru's cmake integration currently not working
# add_subdirectory(           ${BASE_LIB}/loguru )
# target_link_libraries(      aeon-base PRIVATE loguru::loguru )
set( LOGURU_DIR ${BASE_LIB}/loguru )
add_library( loguru STATIC ${LOGURU_DIR}/loguru.hpp ${LOGURU_DIR}/loguru.cpp )
target_link_libraries( loguru PRIVATE dl pthread )