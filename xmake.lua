set_project("magster")

-- Set C++ standard
set_languages("cxx23")

-- Add project modes
add_rules("mode.debug", "mode.release")

add_requires("fmt", "sfml", "nlohmann_json", "tmx", "imgui", "imgui-sfml", "openal-soft")

-- Set debug flags
if is_mode("debug") then
    add_cxflags("-fno-omit-frame-pointer", "-fsanitize=address,undefined")
    add_ldflags("-fno-omit-frame-pointer", "-fsanitize=address,undefined")
end

target("magster")
    if is_plat("macosx") then
        add_ldflags("-framework CoreFoundation", "-framework IOKit", "-framework Cocoa", "-framework CoreGraphics", "-framework Carbon", "-ObjC", "-framework AudioUnit", "-framework CoreAudio", {force = true})
    end
    add_files("src/*.cpp|src/main.cpp")
    add_files("src/main.cpp", {defines = "SRC_DIR=\"" .. os.curdir() .. "\""})
    set_kind("binary")
    -- use lld
    set_toolchains("clang", {ld = "clang++", ldflags = "-fuse-ld=lld"})
    add_files("modules/*")
    add_packages("fmt", "sfml", "nlohmann_json", "tmx", "imgui", "imgui-sfml", "openal-soft")
    add_cxflags("-Wno-deprecated-declarations")

