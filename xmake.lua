---@diagnostic disable: undefined-global, undefined-field
set_project("magster")

add_rules("mode.debug", "mode.release")
set_defaultmode("debug")

set_languages("c++23")
add_requires("raylib", "raygui", "rapidjson", "fmt")
set_policy("build.c++.modules.std", true)

target("vendor")
  set_kind("static")
  -- set_policy("build.c++.modules", true)
  add_packages("raylib", "raygui", "rapidjson","fmt", { public = true })
  add_files("vendor/*.cpp")
  add_files("vendor/*.cppm", { public = true })


target("magsterlib")
  set_kind("static")
  -- set_policy("build.c++.modules", true)
  add_files("src/**.cpp")
  remove_files("src/main.cpp")
  add_files("src/**.cppm", { public = true })
  add_defines(string.format('SRC_DIR="%s"', os.scriptdir()))
  add_deps("vendor")

target("magster")
  set_kind("binary")
  -- set_policy("build.c++.modules.std", true)
  -- set_policy("build.c++.modules", true)
  add_files("src/main.cpp")
  add_defines(string.format('SRC_DIR="%s"', os.scriptdir()))
  add_deps("vendor", "magsterlib")


target("example-gui")
  set_kind("binary")
  -- set_policy("build.c++.modules.std", true)
  -- set_policy("build.c++.modules", true)
  add_files("examples/gui.cpp")
  add_defines(string.format('SRC_DIR="%s"', os.scriptdir()))
  add_deps("magsterlib", "vendor")


  add_rules("mode.debug", "mode.release")


for _, file in ipairs(os.files("test/src/*.cpp")) do
    local name = path.basename(file)
    target(name)
        set_kind("binary")
        set_default(false)
        add_files("test/src/" .. name .. ".cpp", "./test/misc/ut.cppm")
        add_deps("vendor", "magsterlib")
        add_tests("default")
end
