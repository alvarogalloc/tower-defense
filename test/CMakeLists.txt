file(GLOB TEST_MODULES "*.cppm")
add_executable(tests main.cpp)

target_compile_options(tests PRIVATE -fno-omit-frame-pointer
                                     -fsanitize=address,undefined)
target_link_libraries(tests PRIVATE -fno-omit-frame-pointer
                                    -fsanitize=address,undefined)

target_link_libraries(tests PRIVATE gamelib)
target_sources(
  tests
  PUBLIC FILE_SET
         cxx_modules
         TYPE
         CXX_MODULES
         FILES
         ${APP_MODULE_FILES}
         ${TEST_MODULES})
add_test(NAME test COMMAND tests)
