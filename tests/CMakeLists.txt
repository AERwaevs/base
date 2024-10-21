if( BUILD_TESTING )
    add_executable( tests
        #object.test.cpp
        MemorySlots.test.cpp
    )

    target_compile_features(    tests PRIVATE cxx_std_20 )
    target_include_directories( tests PRIVATE ${BASE_INC} )
    target_link_libraries( tests PRIVATE Catch2::Catch2WithMain aer::base )
endif()