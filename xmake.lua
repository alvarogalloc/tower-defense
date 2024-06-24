---@diagnostic disable: undefined-global, undefined-field
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

-- set_toolchains("clang", { ld = "clang++", ldflags = "-fuse-ld=lld " .. os.getenv("LDFLAGS") })
add_cxflags("-Wno-deprecated-declarations")

target("magsterlib")
  set_kind("static")
  add_defines('SRC_DIR=\"' .. os.curdir() .. '\"', {public = true})
  add_files("src/*.cpp|src/main.cpp")
  add_files("modules/*", {public = true})
  add_packages("fmt", "sfml", "nlohmann_json", "tmx", "imgui", "imgui-sfml", "openal-soft", {public = true})
  if is_plat("macosx") then
    add_ldflags(
      "-framework CoreFoundation",
      "-framework IOKit",
      "-framework Cocoa",
      "-framework CoreGraphics",
      "-framework Carbon",
      "-ObjC",
      "-framework AudioUnit",
      "-framework CoreAudio",
      os.getenv("LDFLAGS"),
      { force = true, public = true}
    )
  end

target("magster")
  add_files("src/main.cpp")
  set_kind("binary")
  add_deps("magsterlib")

target("test_magster")
  set_kind("binary")
  set_default(false)
  add_files("test/main.cpp")
  add_files("test/*.cppm")
  add_tests("default")
  add_deps("magsterlib")
