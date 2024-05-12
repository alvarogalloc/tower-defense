cmake_minimum_required(VERSION 3.28)
project(magster)
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
file(GLOB_RECURSE APP_RESOURCES "${CMAKE_SOURCE_DIR}/assets/**")

set(MACOSX_BUNDLE_ICON_FILE "icon.icns")
set(APP_ICON "${CMAKE_SOURCE_DIR}/assets/${MACOSX_BUNDLE_ICON_FILE}")
set_source_files_properties(${APP_ICON}
    PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
)

add_executable(${CMAKE_PROJECT_NAME} ${APP_SOURCE_FILES} ${APP_RESOURCES})
target_compile_features(${CMAKE_PROJECT_NAME} PUBLIC cxx_std_23)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES CXX_STANDARD 23)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE fmt::fmt)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE tmx)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
#ignore deprecated declarations
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wno-deprecated-declarations)

target_sources(${CMAKE_PROJECT_NAME} PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES
  ${APP_MODULE_FILES})

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,undefined")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,undefined")


# configs
option(BUILD_BUNDLE "Build bundle" OFF)

if (APPLE AND BUILD_BUNDLE)
    set_target_properties(${CMAKE_PROJECT_NAME}
        PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_ICON_FILE ${MACOSX_BUNDLE_ICON_FILE}
            MACOSX_BUNDLE_BUNDLE_NAME "${CMAKE_PROJECT_NAME}" # CFBundleIdentifier
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.alvarogallo"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1" # CFBundleShortVersionString
            # and bundle resources again
            # RESOURCE "${APP_RESOURCES}"
    )
    add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets
    "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/../Resources/assets")
else()
      add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets
    "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/assets")

endif()


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
