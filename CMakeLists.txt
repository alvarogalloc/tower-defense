cmake_minimum_required(VERSION 3.30 FATAL_ERROR)
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")
set(CMAKE_CXX_MODULE_STD ON)

project(magster LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
# Currently CMake requires extensions enabled when using import std.
# https://gitlab.kitware.com/cmake/cmake/-/issues/25916
# https://gitlab.kitware.com/cmake/cmake/-/issues/25539
set(CMAKE_CXX_EXTENSIONS ON)
# set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations -std=gnu++23")

# use clang tidy find_program(CLANG_TIDY_EXE NAMES "clang-tidy" QUIET)
#
# if(CLANG_TIDY_EXE) set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}")
# macro(use_clang_tidy TARGET) set_target_properties(${TARGET} PROPERTIES
# CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}") endmacro() else() message(WARNING
# "clang-tidy not found. Code analysis will be skipped.") endif()



find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
find_path(RAYGUI_INCLUDE_DIRS "raygui.h")
# todo change this when module support
# find_package(glaze CONFIG REQUIRED)
# find_package(nlohmann_json CONFIG REQUIRED)

add_compile_definitions(SRC_DIR="${CMAKE_SOURCE_DIR}")
file(GLOB_RECURSE GAMELIB_MODULE_FILES "src/*.cppm")
file(GLOB_RECURSE GAMELIB_SOURCE_FILES "src/*.cpp")
list(REMOVE_ITEM GAMELIB_SOURCE_FILES "src/main.cpp")
add_library(gamelib)
# use_clang_tidy(gamelib)
file(GLOB_RECURSE VENDOR_MODULES "vendor/*.cppm")
file(GLOB_RECURSE VENDOR_SOURCE_FILES "vendor/*.cpp")
add_library(vendor)
target_sources(
  vendor
  PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES ${VENDOR_MODULES}
  PRIVATE ${VENDOR_SOURCE_FILES})
target_link_libraries(vendor PUBLIC raylib)
# make these use c++23 
target_include_directories(vendor SYSTEM PUBLIC ${RAYGUI_INCLUDE_DIRS})# suprress header warnings

target_link_libraries(vendor PUBLIC glfw)
target_compile_features(vendor PRIVATE cxx_std_23)
# target_link_libraries(vendor PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(vendor PRIVATE RapidJSON rapidjson)
target_link_libraries(vendor PUBLIC fmt::fmt)
target_link_libraries(gamelib PUBLIC vendor)

target_compile_features(gamelib PUBLIC cxx_std_23)
# debug use sanitizers
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(gamelib PUBLIC -fsanitize=address -fsanitize=leak
                                        -fsanitize=undefined)
  target_link_options(gamelib PUBLIC -fsanitize=address -fsanitize=leak
                      -fsanitize=undefined)
  # add libc++ hardening 
add_definitions(-D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_DEBUG)
endif()
target_sources(
  gamelib
  PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES ${GAMELIB_MODULE_FILES}
  PRIVATE ${GAMELIB_SOURCE_FILES})

target_compile_options(
  gamelib
  PUBLIC
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
    -Wall
    -Wextra
    -Werror
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wcast-align
    -Wunused
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -pedantic-errors>
    $<$<CXX_COMPILER_ID:MSVC>:
    /W4>)

set(GAME_SOURCE_FILES "src/main.cpp")
file(GLOB_RECURSE GAME_RESOURCES "${CMAKE_SOURCE_DIR}/assets/*")

add_executable(magster ${GAME_SOURCE_FILES} ${GAME_RESOURCES})
# use_clang_tidy(gamelib) add a macro with the current path
target_link_libraries(magster PRIVATE gamelib)

# if option TESTS is enabled add test for each file in test folder
option(BUILD_TESTING "Enable Test Builds" OFF)
if(FALSE AND BUILD_TESTING)
  enable_testing()

  add_subdirectory(test)
endif()

# if option EXAMPLES is enabled add example for each file in examples folder
option(BUILD_EXAMPLES "Enable Example Builds" OFF)
if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
