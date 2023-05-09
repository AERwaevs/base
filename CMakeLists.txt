# cmake -------------------------------------------------------------------------------------------
cmake_minimum_required( VERSION 3.2.0 )

set( CMAKE_CXX_STANDARD          20  )
set( CMAKE_CXX_STANDARD_REQUIRED ON  )
set( CMAKE_CXX_EXTENSIONS        OFF )
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

add_library(                aeon-base STATIC ${HEADERS} )
target_include_directories( aeon-base PUBLIC ${BASE_INC} PRIVATE ${BASE_LIB} )
target_precompile_headers(  aeon-base PUBLIC ${STD_HEADERS} )
target_compile_features(    aeon-base PUBLIC cxx_std_20)
# submodules --------------------------------------------------------------------------------------
find_package( Git QUIET )
if( GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git" )
    option( GIT_SUBMODULE "Check submodules during build" ON )
    if( GIT_SUBMODULE )
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()
# loguru ------------------------------------------------------------------------------------------
add_subdirectory(           ${BASE_LIB}/loguru )
add_dependencies(           aeon-base loguru::loguru )
target_link_libraries(      aeon-base PRIVATE loguru::loguru )