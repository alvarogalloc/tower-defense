file(GLOB TEST_SOURCES "src/*.cpp")

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE) # name without extension
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_compile_features(${TEST_NAME} PUBLIC cxx_std_23)
    target_compile_options(${TEST_NAME} PRIVATE -fno-omit-frame-pointer
                                               )
    target_link_libraries(${TEST_NAME} PRIVATE -fno-omit-frame-pointer
                                              gamelib)

    target_sources(
        ${TEST_NAME}
        PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES misc/ut.cppm misc/testutils.cppm
    )
    set_source_files_properties(misc/ut.cppm 
        PROPERTIES
        COMPILE_FLAGS "-Wno-implicit-float-conversion -Wno-implicit-int-conversion -Wno-sign-conversion -Wno-shorten-64-to-32 -Wno-old-style-cast"
    )
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
