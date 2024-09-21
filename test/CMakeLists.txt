file(GLOB TEST_MODULES "./modules/*.cppm")
file(GLOB TEST_SOURCES "./src/*.cpp")
add_executable(tests)
target_compile_features(tests PRIVATE cxx_std_23)
target_compile_options(tests PRIVATE -fno-omit-frame-pointer
                                     -fsanitize=address,undefined)
target_link_libraries(tests PRIVATE -fno-omit-frame-pointer
                                    -fsanitize=address,undefined)

target_link_libraries(tests PRIVATE gamelib)
target_sources(
  tests
  PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES ${TEST_MODULES}
  PRIVATE ${TEST_SOURCES})
add_test(NAME test COMMAND tests)
