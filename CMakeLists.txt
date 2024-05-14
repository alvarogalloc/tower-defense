cmake_minimum_required(VERSION 3.28)
project(magster)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,undefined")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,undefined")

find_package(fmt CONFIG REQUIRED)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(tmx CONFIG REQUIRED)

file(GLOB GAMELIB_MODULE_FILES "modules/*.cppm")
set(GAMELIB_SOURCE_FILES "src/tilemap.cpp")

add_library(gamelib ${GAMELIB_SOURCE_FILES})
target_compile_features(gamelib PUBLIC cxx_std_23)
target_sources(gamelib PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES
  ${GAMELIB_MODULE_FILES})
target_link_libraries(gamelib PUBLIC sfml-system sfml-network sfml-graphics sfml-window)
target_link_libraries(gamelib PUBLIC fmt::fmt)
target_link_libraries(gamelib PUBLIC tmx)
target_link_libraries(gamelib PUBLIC nlohmann_json::nlohmann_json)
target_compile_options(gamelib PUBLIC -Wno-deprecated-declarations)


set(GAME_SOURCE_FILES "src/main.cpp")
file(GLOB_RECURSE GAME_RESOURCES "${CMAKE_SOURCE_DIR}/assets/**")

add_executable(magster ${GAME_SOURCE_FILES} ${GAME_RESOURCES})
target_link_libraries(magster PRIVATE gamelib)
# configs
option(BUILD_BUNDLE "Build bundle" OFF)

if (APPLE AND BUILD_BUNDLE)
    set(MACOSX_BUNDLE_ICON_FILE "icon.icns")
    set(APP_ICON "${CMAKE_SOURCE_DIR}/assets/${MACOSX_BUNDLE_ICON_FILE}")
    set_source_files_properties(${APP_ICON}
        PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources"
    )
    set_target_properties(magster
        PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_ICON_FILE ${MACOSX_BUNDLE_ICON_FILE}
            MACOSX_BUNDLE_BUNDLE_NAME "magster" # CFBundleIdentifier
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.alvarogallo"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1" # CFBundleShortVersionString
            # and bundle resources again
            # RESOURCE "${APP_RESOURCES}"
    )
    add_custom_command(
    TARGET magster
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets
    "$<TARGET_FILE_DIR:magster>/../Resources/assets")
else()
      add_custom_command(
    TARGET magster
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets
    "$<TARGET_FILE_DIR:magster>/assets")

endif()


# if option TESTS is enabled
# add test for each file in test folder
option(BUILD_TESTING "Enable Test Builds" OFF)
if(BUILD_TESTING)
    find_package(doctest CONFIG REQUIRED)
    enable_testing()
    file(GLOB TEST_SOURCES "test/*.cpp")
    find_package(Catch2 CONFIG REQUIRED)
    foreach(TEST_SOURCE ${TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
        add_executable(${TEST_NAME} ${TEST_SOURCE})
        target_link_libraries(${TEST_NAME} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
        target_link_libraries(${TEST_NAME} PRIVATE gamelib)
        target_sources(${TEST_NAME} PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES
            ${APP_MODULE_FILES})
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endforeach()
endif()
