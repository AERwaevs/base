# cmake -------------------------------------------------------------------------------------------
cmake_minimum_required( VERSION 3.20 )
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
    ${BASE_SRC}/Event.h
    ${BASE_SRC}/EventListener.h
    ${BASE_SRC}/interface.h
    ${BASE_SRC}/log.h
    ${BASE_SRC}/platform.h
    ${BASE_SRC}/interfaces/create.h
    ${BASE_SRC}/interfaces/singleton.h
    ${BASE_SRC}/interfaces/type_info.h
    ${BASE_SRC}/memory/object.h
    ${BASE_SRC}/memory/scratch_memory.h
    ${BASE_SRC}/memory/ref_ptr.h
    ${BASE_SRC}/memory/spy_ptr.h
    ${BASE_SRC}/io/Read.h
    ${BASE_SRC}/io/Write.h
    ${BASE_SRC}/io/ReaderWriter.h
)

set( LOGURU_SRC 
    ${BASE_LIB}/loguru/loguru.hpp 
    ${BASE_LIB}/loguru/loguru.cpp
)

add_library( base STATIC ${HEADERS} ${LOGURU_SRC} )
target_precompile_headers(  base PUBLIC ${STD_HEADERS} )
target_compile_features(    base PUBLIC cxx_std_20 )
target_include_directories( base 
    PUBLIC
        $<BUILD_INTERFACE:${BASE_INC}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        $<BUILD_INTERFACE:${BASE_LIB}>
)


add_library( aer::base ALIAS base )