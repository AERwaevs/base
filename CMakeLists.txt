cmake_minimum_required( VERSION 3.20 )

if(NOT DEFINED PROJECT_NAME)
  set(NOT_SUBPROJECT ON)
else()
  set(NOT_SUBPROJECT OFF)
endif()

project( AER::base
    VERSION 0.1.0
    DESCRIPTION "Base library for AER"
    LANGUAGES CXX
)

include(FetchContent)
set( FETCHCONTENT_QUIET FALSE )

set( BUILD_TESTING               OFF CACHE BOOL "Enable testing" )
# loguru -----------------------------------------------------------------------------------------

FetchContent_Declare( loguru
	GIT_REPOSITORY https://github.com/emilk/loguru # can be a filesystem path
	GIT_TAG        master
)
set( LOGURU_WITH_STREAMS TRUE )
FetchContent_MakeAvailable( loguru )

# AER::Base --------------------------------------------------------------------------------------
set( INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc )
set( BASE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )

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
    ${INC_DIR}/Base/Base.h
    ${INC_DIR}/Base/Event.h
    ${INC_DIR}/Base/EventListener.h
    ${INC_DIR}/Base/object.h
    ${INC_DIR}/Base/inherit.h
    ${INC_DIR}/Base/platform.h
    ${INC_DIR}/Base/type_name.h
    ${INC_DIR}/Base/thread_utils.h
    
    ${INC_DIR}/Base/memory/MemoryTracking.h
    ${INC_DIR}/Base/memory/MemoryBlock.h
    ${INC_DIR}/Base/memory/MemoryBlocks.h
    ${INC_DIR}/Base/memory/MemorySlots.h
    ${INC_DIR}/Base/memory/Allocator.h
    ${INC_DIR}/Base/memory/AllocatorPolicy.h
    ${INC_DIR}/Base/memory/Manager.h
    
    ${INC_DIR}/Base/memory/scratch_memory.h
    ${INC_DIR}/Base/memory/base_ptr.h
    ${INC_DIR}/Base/memory/ref_ptr.h
    ${INC_DIR}/Base/memory/spy_ptr.h

    ${INC_DIR}/Base/nodes/Node.h
    ${INC_DIR}/Base/nodes/Group.h
)

set( SOURCES 
    ${BASE_SOURCE_DIR}/Allocator.cpp
    ${BASE_SOURCE_DIR}/MemoryBlock.cpp
    ${BASE_SOURCE_DIR}/MemoryBlocks.cpp
    ${BASE_SOURCE_DIR}/MemorySlots.cpp
)

add_library( base ${HEADERS} ${SOURCES} )

set_target_properties( base PROPERTIES
    CXX_STANDARD                    23
    CXX_STANDARD_REQUIRED           ON
    CXX_EXTENSIONS                  OFF
    CMAKE_POSITION_INDEPENDENT_CODE ON
    FOLDER                          "AER"
)

target_precompile_headers(  base PUBLIC ${STD_HEADERS} )
target_include_directories( base 
    PUBLIC
        $<BUILD_INTERFACE:${INC_DIR}>
)
target_link_libraries( base PUBLIC loguru::loguru )

add_library( aer::base ALIAS base )
set( base_FOUND TRUE CACHE INTERNAL "aer::base found." )
set( CMAKE_DISABLE_FIND_PACKAGE_base TRUE CACHE INTERNAL "Disable find_package(base) as it is not necessary." )

# Testing -----------------------------------------------------------------------------------------
if( CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME )
    include(CTest)
    if( BUILD_TESTING )
        add_subdirectory( test )
    endif()
endif()