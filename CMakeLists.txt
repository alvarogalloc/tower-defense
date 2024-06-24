cmake_minimum_required(VERSION 3.28)
project(magster)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
find_package(fmt CONFIG REQUIRED)
find_package(
  SFML
  COMPONENTS system audio window graphics CONFIG
  REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(tmx CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)

add_compile_definitions(SRC_DIR="${CMAKE_SOURCE_DIR}")
file(GLOB GAMELIB_MODULE_FILES "modules/*.cppm")
file(GLOB GAMELIB_SOURCE_FILES "src/*.cpp")
list(REMOVE_ITEM GAMELIB_SOURCE_FILES "src/main.cpp")

add_library(gamelib ${GAMELIB_SOURCE_FILES})
target_compile_features(gamelib PUBLIC cxx_std_23)
target_sources(gamelib PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES
                              ${GAMELIB_MODULE_FILES})
target_link_libraries(gamelib PUBLIC sfml-system sfml-network sfml-graphics
                                     sfml-window sfml-audio)
target_link_libraries(gamelib PUBLIC OpenAL::OpenAL)
target_link_libraries(gamelib PUBLIC ImGui-SFML::ImGui-SFML)
target_link_libraries(gamelib PUBLIC imgui::imgui)

target_link_libraries(gamelib PUBLIC fmt::fmt)
target_link_libraries(gamelib PUBLIC tmx)
target_link_libraries(gamelib PRIVATE nlohmann_json::nlohmann_json)
target_compile_options(gamelib PUBLIC -Wno-deprecated-declarations)

set(GAME_SOURCE_FILES "src/main.cpp")
file(GLOB_RECURSE GAME_RESOURCES "${CMAKE_SOURCE_DIR}/assets/*")

add_executable(magster ${GAME_SOURCE_FILES} ${GAME_RESOURCES})
# add a macro with the current path
target_link_libraries(magster PRIVATE gamelib)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(magster PRIVATE -fno-omit-frame-pointer
                                         -fsanitize=address,undefined)
  target_link_libraries(magster PRIVATE -fno-omit-frame-pointer
                                        -fsanitize=address,undefined)
endif()

# if option TESTS is enabled add test for each file in test folder
option(BUILD_TESTING "Enable Test Builds" OFF)
if(BUILD_TESTING)
  enable_testing()

  add_subdirectory(test)
endif()
