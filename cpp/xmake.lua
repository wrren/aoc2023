add_rules("mode.debug", "mode.release")

add_includedirs("include")
set_languages("cxx20")

target("day1")
    set_kind("binary")
    add_files("src/day1/*.cpp")

target("day2")
    set_kind("binary")
    add_files("src/day2/*.cpp")

target("day3")
    set_kind("binary")
    add_files("src/day3/*.cpp")

target("day4")
    set_kind("binary")
    add_files("src/day4/*.cpp")

target("day5")
    set_kind("binary")
    add_files("src/day5/*.cpp")

target("day6")
    set_kind("binary")
    add_files("src/day6/*.cpp")

target("day7")
    set_kind("binary")
    add_files("src/day7/*.cpp")

target("day8")
    set_kind("binary")
    add_files("src/day8/*.cpp")

target("day9")
    set_kind("binary")
    add_files("src/day9/*.cpp")

target("day10")
    set_kind("binary")
    add_files("src/day10/*.cpp")

--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro definition
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

