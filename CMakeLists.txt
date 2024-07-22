cmake_minimum_required(VERSION 3.28)
project(magster)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")


# 10.2.1 fmt
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master)
set(FMT_USE_CMAKE_MODULES
    ON
    CACHE BOOL "Use CMake modules for fmt")
set(FMT_MODULE
    ON
    CACHE BOOL "Use fmt as a CMake module")
FetchContent_MakeAvailable(fmt)

find_package(RapidJSON CONFIG REQUIRED)
find_package(tmx CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(raylib CONFIG REQUIRED)
find_path(RAYGUI_INCLUDE_DIRS "raygui.h")

add_compile_definitions(SRC_DIR="${CMAKE_SOURCE_DIR}")
file(GLOB GAMELIB_MODULE_FILES "modules/*.cppm")
file(GLOB GAMELIB_SOURCE_FILES "src/*.cpp")
list(REMOVE_ITEM GAMELIB_SOURCE_FILES "src/main.cpp")
add_library(gamelib)

file(GLOB_RECURSE VENDOR_MODULES "vendor/modules/*.cppm")
file(GLOB_RECURSE VENDOR_SOURCE_FILES "vendor/src/*.cpp")
add_library(vendor)
target_sources(vendor PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES
  ${VENDOR_MODULES} PRIVATE ${VENDOR_SOURCE_FILES})
target_link_libraries(vendor PUBLIC raylib)
target_include_directories(vendor SYSTEM INTERFACE ${RAYGUI_INCLUDE_DIRS})
target_link_libraries(vendor PUBLIC fmt::fmt)
target_link_libraries(vendor PUBLIC tmx)
target_link_libraries(vendor PUBLIC rapidjson)

target_link_libraries(gamelib PRIVATE vendor)


target_compile_features(gamelib PUBLIC cxx_std_23)
# debug use sanitizers
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(gamelib PUBLIC -fsanitize=address -fsanitize=leak
                                         -fsanitize=undefined)
  target_link_options(gamelib PUBLIC -fsanitize=address -fsanitize=leak
                                      -fsanitize=undefined)
endif()
target_sources(gamelib PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES
  ${GAMELIB_MODULE_FILES} PRIVATE ${GAMELIB_SOURCE_FILES})


set(GAME_SOURCE_FILES "src/main.cpp")
file(GLOB_RECURSE GAME_RESOURCES "${CMAKE_SOURCE_DIR}/assets/*")

add_executable(magster ${GAME_SOURCE_FILES} ${GAME_RESOURCES})
# add a macro with the current path
target_link_libraries(magster PRIVATE gamelib)

# if option TESTS is enabled add test for each file in test folder
option(BUILD_TESTING "Enable Test Builds" OFF)
# if(BUILD_TESTING)
#   enable_testing()
#
#   add_subdirectory(test)
# endif()
