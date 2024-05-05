cmake_minimum_required(VERSION 3.28)
project(usingmodules)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

find_package(fmt CONFIG REQUIRED)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(tmx CONFIG REQUIRED)

file(GLOB APP_MODULE_FILES "modules/*.cppm")
file(GLOB APP_SOURCE_FILES "src/*.cpp")

add_executable(app ${APP_SOURCE_FILES} ${APP_MODULE_FILES})
target_link_libraries(app PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
target_link_libraries(app PRIVATE fmt::fmt)
target_link_libraries(app PRIVATE tmx)
target_link_libraries(app PRIVATE nlohmann_json::nlohmann_json)
#ignore deprecated declarations
target_compile_options(app PRIVATE -Wno-deprecated-declarations)

target_sources(app PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES
  ${APP_MODULE_FILES})

# if option TESTS is enabled
# add test for each file in test folder
option(BUILD_TESTING "Enable Test Builds" OFF)
if(BUILD_TESTING)
    find_package(ut CONFIG REQUIRED)
    enable_testing()
    file(GLOB TEST_SOURCES "test/*.cpp")
    foreach(TEST_SOURCE ${TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
        add_executable(${TEST_NAME} ${TEST_SOURCE})
        target_link_libraries(${TEST_NAME} PRIVATE engine Boost::ut )
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endforeach()
endif()
