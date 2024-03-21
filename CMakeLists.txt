# cmake -------------------------------------------------------------------------------------------
cmake_minimum_required( VERSION 3.20 )

project( aer::base
    VERSION 0.1.0
    LANGUAGES CXX
)

set( CMAKE_CXX_STANDARD          23  )
set( CMAKE_CXX_STANDARD_REQUIRED ON  )
set( CMAKE_CXX_EXTENSIONS        OFF )
set( CMAKE_POSITION_INDEPENDENT_CODE ON )

if( ANDROID )
    set( AER_ANDROID 1 )
elseif( WIN32 )
    set( AER_WIN32 1 )
elseif ( APPLE )
    set( AER_APPLE 1 )
else()
    set( AER_LINUX 1 )
endif()

# AEON::Base --------------------------------------------------------------------------------------
set( BASE_LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib )
set( BASE_INC ${CMAKE_CURRENT_SOURCE_DIR}/inc )
set( BASE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src )

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
    ${BASE_INC}/Base/Base.h
    ${BASE_INC}/Base/Event.h
    ${BASE_INC}/Base/EventListener.h
    ${BASE_INC}/Base/interface.h
    ${BASE_INC}/Base/object.h
    ${BASE_INC}/Base/log.h
    ${BASE_INC}/Base/platform.h
    ${BASE_INC}/Base/interfaces/create.h
    ${BASE_INC}/Base/interfaces/singleton.h
    ${BASE_INC}/Base/interfaces/type_info.h
    ${BASE_INC}/Base/memory/Allocator.h
    ${BASE_INC}/Base/memory/scratch_memory.h
    ${BASE_INC}/Base/memory/base_ptr.h
    ${BASE_INC}/Base/memory/ref_ptr.h
    ${BASE_INC}/Base/memory/spy_ptr.h
    ${BASE_INC}/Base/io/Read.h
    ${BASE_INC}/Base/io/Write.h
    ${BASE_INC}/Base/io/ReaderWriter.h
)

set( SOURCES 
    ${BASE_SRC}/Allocator.cpp
    ${BASE_SRC}/MemoryBlock.cpp
    ${BASE_SRC}/MemoryBlocks.cpp
    ${BASE_SRC}/MemorySlots.cpp
)

set( LOGURU_SRC 
    ${BASE_LIB}/loguru/loguru.hpp 
    ${BASE_LIB}/loguru/loguru.cpp
)

add_library( base STATIC ${HEADERS} ${SOURCES} ${LOGURU_SRC} )
target_precompile_headers(  base PUBLIC ${STD_HEADERS} )
target_compile_features(    base PUBLIC cxx_std_23 )
target_include_directories( base 
    PUBLIC
        $<BUILD_INTERFACE:${BASE_INC}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        $<BUILD_INTERFACE:${BASE_LIB}>
)

add_library( aer::base ALIAS base )

if( BUILD_TESTING )
    add_subdirectory( lib/catch )
    set( BASE_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test )
    
    add_executable( tests
        ${BASE_TEST_DIR}/MemorySlots_test.cpp
    )

    target_compile_features(    tests PRIVATE cxx_std_20 )
    target_include_directories( tests PRIVATE ${BASE_INC} )
    target_link_libraries( tests PRIVATE Catch2::Catch2 )
endif()