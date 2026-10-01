add_requires("doctest")
add_rules("mode.debug", "mode.release")
set_languages("c++23")
set_policy("build.c++.modules.std", false)
set_policy("build.c++.modules.clang.fallbackscanner", true)

target("celine_lib")
    set_kind("$(kind)")
    add_files("src/celine/*.cppm", {public = true}) -- {public = true} was the problem
    -- add_includedirs("src/celine", {public = true})

    set_toolchains("clang")
    add_cxflags("-stdlib=libc++")
    add_ldflags("-stdlib=libc++")

    -- after_build(function (target)
    --     import("devel.debugger").generate_compile_commnands()
    -- end)


target("celine_bin")
    set_kind("binary")
    add_deps("celine_lib")
    add_files("src/main.cpp")

    set_toolchains("clang")
    add_cxflags("-stdlib=libc++")
    add_ldflags("-stdlib=libc++")


target("celine_tests")
    set_kind("binary") -- No building when just `xmake`
    set_default(false)

    add_deps("celine_lib")
    add_packages("doctest") -- Pull the testing framework

    add_files("src/tests/*.cpp")

    set_toolchains("clang")
    add_cxflags("-stdlib=libc++")
    add_ldflags("-stdlib=libc++")

    add_tests("unit_tests") -- registers target for `xmake test`
