if( BUILD_TESTING )
    find_package( Catch2 REQUIRED )
    set( BASE_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test )
    
    add_executable( tests
    )

    target_compile_features(    tests PRIVATE cxx_std_20 )
    target_include_directories( tests PRIVATE ${BASE_INC} )
    target_link_libraries( tests PRIVATE Catch2::Catch2 )
endif()