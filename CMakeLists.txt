# cmake -------------------------------------------------------------------------------------------
cmake_minimum_required( VERSION 3.20 )

if(NOT DEFINED PROJECT_NAME)
  set(NOT_SUBPROJECT ON)
else()
  set(NOT_SUBPROJECT OFF)
endif()

project( AER::Base
    VERSION 0.1.0
    LANGUAGES CXX
)

set( CMAKE_CXX_STANDARD          23  )
set( CMAKE_CXX_STANDARD_REQUIRED ON  )
set( CMAKE_CXX_EXTENSIONS        OFF )
set( CMAKE_POSITION_INDEPENDENT_CODE ON )
set( BUILD_TESTING               OFF CACHE BOOL "Enable testing" )

if( ANDROID )
    set( AER_ANDROID 1 )
elseif( WIN32 )
    set( AER_WIN32 1 )
elseif ( APPLE )
    set( AER_APPLE 1 )
else()
    set( AER_LINUX 1 )
endif()

# AER::Base --------------------------------------------------------------------------------------
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
    ${BASE_INC}/Base/object.h
    ${BASE_INC}/Base/inherit.h
    ${BASE_INC}/Base/node.h
    ${BASE_INC}/Base/log.h
    ${BASE_INC}/Base/platform.h
    ${BASE_INC}/Base/type_name.h
    ${BASE_INC}/Base/thread_utils.h
    
    ${BASE_INC}/Base/memory/MemoryTracking.h
    ${BASE_INC}/Base/memory/MemoryBlock.h
    ${BASE_INC}/Base/memory/MemoryBlocks.h
    ${BASE_INC}/Base/memory/MemorySlots.h
    ${BASE_INC}/Base/memory/Allocator.h
    ${BASE_INC}/Base/memory/AllocatorPolicy.h
    ${BASE_INC}/Base/memory/Manager.h

    ${BASE_INC}/Base/memory/scratch_memory.h
    ${BASE_INC}/Base/memory/base_ptr.h
    ${BASE_INC}/Base/memory/ref_ptr.h
    ${BASE_INC}/Base/memory/spy_ptr.h
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