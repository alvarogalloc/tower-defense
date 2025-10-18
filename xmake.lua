---@diagnostic disable: undefined-global, undefined-field
set_project("magster")
set_values("c++.clang.module.stdmodules", true)
add_rules("mode.debug", "mode.release")
set_defaultmode("debug")
set_languages("c++latest")
--  use libc++
add_ldflags("-stdlib=libc++")

add_requires("raylib", "raygui", "glaze", "fmt", "glfw")

target("vendor")
  set_kind("static")
  add_packages("raylib", "raygui", "glaze","fmt","glfw", { public = true })
  add_files("vendor/*.cpp")
  add_files("vendor/*.cppm", { public = true })


target("magsterlib")
  set_kind("static")
  add_files("src/**.cpp")
  remove_files("src/main.cpp")
  add_files("src/**.cppm", { public = true })
  add_defines(string.format('SRC_DIR="%s"', os.scriptdir()))
  add_deps("vendor")

target("magster")
  set_kind("binary")
  add_files("src/main.cpp")
  add_defines(string.format('SRC_DIR="%s"', os.scriptdir()))
  add_deps("vendor", "magsterlib")


for _, file in ipairs(os.files("test/src/*.cpp")) do
    local name = path.basename(file)
    target(name)
        set_kind("binary")
        set_default(false)
        add_files("test/src/" .. name .. ".cpp", "./test/misc/ut.cppm")
        add_deps("vendor", "magsterlib")
        add_tests("default")
end
