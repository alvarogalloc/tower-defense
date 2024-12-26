---@diagnostic disable: undefined-global, undefined-field
set_project("magster")

-- Set C++ standard
set_languages("cxx23")

-- Add project modes
add_rules("mode.debug", "mode.release")

add_requires("raylib", "raygui", "rapidjson")

-- Set debug flags
if is_mode("debug") then
	add_cxflags("-fno-omit-frame-pointer", "-fsanitize=address,undefined")
	add_ldflags("-fno-omit-frame-pointer", "-fsanitize=address,undefined")
end

add_cxflags("-Wno-deprecated-declarations")

target("magster")
  add_files("src/*.cpp")
  add_files("src/*.cppm")
  set_kind("binary")
  add_deps("raylib", "raygui", "rapidjson")
