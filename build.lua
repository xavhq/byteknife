#!/usr/bin/env lua
-- build.lua — build helper for byteknife
--
-- Usage:
--   ./build.lua           -> configure + build (Release)
--   ./build.lua debug     -> configure + build (Debug)
--   ./build.lua run       -> configure + build (Release) then run the executable
--   ./build.lua clean     -> remove the build directory

local BUILD_DIR = "build"
local PROGRAM = "./" .. BUILD_DIR .. "/dist/byteknife"
local BUILD_TYPE = "Release"

local function run(cmd)
    print("> " .. cmd)
    local ok = os.execute(cmd)
    return ok == true or ok == 0
end

local function nproc()
    local f = io.popen("nproc")
    local n = f:read("*l")
    f:close()
    return n or "4"
end

local function build()
    print("[CONFIGURE] (" .. BUILD_TYPE .. ")")
    os.execute("mkdir -p " .. BUILD_DIR)

    local configure_ok = run(
        "cmake -S . -B " .. BUILD_DIR ..
        " -DCMAKE_BUILD_TYPE=" .. BUILD_TYPE ..
        " -DCMAKE_C_COMPILER=clang" ..
        " -DCMAKE_CXX_COMPILER=clang++"
    )
    if not configure_ok then
        print("[ERROR] configure failed")
        os.exit(1)
    end

    print("[BUILD]")
    local build_ok = run("cmake --build " .. BUILD_DIR .. " -j" .. nproc())
    if not build_ok then
        print("[ERROR] build failed")
        os.exit(1)
    end

    print("[OK] build complete")
end

local arg1 = arg[1]

if arg1 == "debug" then
    BUILD_TYPE = "Debug"
    PROGRAM = "./" .. BUILD_DIR .. "/dist/byteknife"
    build()
    os.exit(0)
end

if arg1 == "clean" then
    print("[CLEAN]")
    os.execute("rm -rf " .. BUILD_DIR)
    print("[OK] clean complete")
    os.exit(0)
end

if arg1 == "run" then
    build()
    local f = io.open(PROGRAM, "r")
    if f then
        f:close()
        print("[RUN] " .. PROGRAM)
        os.execute(PROGRAM)
    else
        print("[ERROR] executable not found at " .. PROGRAM)
        os.exit(1)
    end
    os.exit(0)
end

build()
