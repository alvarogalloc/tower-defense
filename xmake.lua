---@diagnostic disable: undefined-global, undefined-field
set_project("magster")
add_rules("mode.debug", "mode.release")
set_defaultmode("debug")
set_languages("c++latest")
--  use libc++
-- add_ldflags("-stdlib=libc++")

set_toolchains("llvm")
set_runtimes("c++_shared")
add_syslinks("c++abi")
add_requires("raylib", "raygui", "glaze", "glfw", "entt", "cpptrace")

target("vendor")
do
	set_kind("static")
	add_packages("raylib", "raygui", "glaze", "glfw", "entt", "cpptrace", { public = true })
	add_files("vendor/*.cpp")
	add_files("vendor/*.cppm", { public = true })
end

target("magsterlib")
do
	set_kind("static")
	add_files(
		"src/components/*.cpp",
		"src/wey/*.cpp",
		"src/gui/*.cpp",
		"src/prefabs/*.cpp",
		"src/scenes/*.cpp",
		"src/systems/*.cpp"
	)
	remove_files("src/main.cpp")
	add_files(
		"src/wey/*.cppm",
		"src/components/*.cppm",
		"src/gui/*.cppm",
		"src/prefabs/*.cppm",
		"src/scenes/*.cppm",
		"src/systems/*.cppm",
		{ public = true }
	)
	add_deps("vendor")
end

target("magster")
do
	set_kind("binary")
	add_files("src/main.cpp")
  set_policy("build.c++.modules", true)
	add_deps("vendor", "magsterlib")
end

-- for _, file in ipairs(os.files("test/src/*.cpp")) do
--     local name = path.basename(file)
--     target(name)
--         set_kind("binary")
--         set_default(false)
--         add_files("test/src/" .. name .. ".cpp", "./test/misc/ut.cppm")
--         add_deps("vendor", "magsterlib")
--         add_tests("default")
-- end
