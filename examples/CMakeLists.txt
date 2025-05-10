#add a exe for each file, name convention is 'name.example'

file(GLOB EXAMPLES "*.cpp")

foreach(EXAMPLE ${EXAMPLES})
    get_filename_component(EXAMPLE_NAME "${EXAMPLE}" NAME_WE)

    add_executable("${EXAMPLE_NAME}.example" "${EXAMPLE}")

    target_compile_features("${EXAMPLE_NAME}.example" PRIVATE cxx_std_23)

    target_compile_options("${EXAMPLE_NAME}.example" PRIVATE
        -fno-omit-frame-pointer
        -fsanitize=address,undefined)

    target_link_options("${EXAMPLE_NAME}.example" PRIVATE
        -fsanitize=address,undefined)

    target_link_libraries("${EXAMPLE_NAME}.example" PRIVATE gamelib)
endforeach()
