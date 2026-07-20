#!/usr/bin/env lua
-- build.lua — build helper for byteknife
--
-- Usage:
--   ./build.lua           -> configure + build (Release)
--   ./build.lua debug     -> configure + build (Debug)
--   ./build.lua run       -> configure + build (Release) then run the executable
--   ./build.lua clean     -> remove the build directory
--
-- On Windows, run as: lua build.lua <command>   (no ./ , no exec bit needed)

local IS_WINDOWS = package.config:sub(1, 1) == "\\"

local BUILD_DIR = "build"
local BUILD_TYPE = "Release"

local function program_path()
    local exe = IS_WINDOWS and "byteknife.exe" or "byteknife"
    return (IS_WINDOWS and "" or "./") .. BUILD_DIR .. "/dist/" .. exe
end

local function run(cmd)
    print("> " .. cmd)
    local ok = os.execute(cmd)
    return ok == true or ok == 0
end

local function nproc()
    if IS_WINDOWS then
        local n = os.getenv("NUMBER_OF_PROCESSORS")
        return n or "4"
    end
    local f = io.popen("nproc")
    if not f then return "4" end
    local n = f:read("*l")
    f:close()
    return n or "4"
end

local function ensure_build_dir()
    if IS_WINDOWS then
        os.execute('if not exist "' .. BUILD_DIR .. '" mkdir "' .. BUILD_DIR .. '"')
    else
        os.execute("mkdir -p " .. BUILD_DIR)
    end
end

local function build()
    print("[CONFIGURE] (" .. BUILD_TYPE .. ")")
    ensure_build_dir()

    local configure_cmd = "cmake -S . -B " .. BUILD_DIR ..
        " -DCMAKE_BUILD_TYPE=" .. BUILD_TYPE ..
        " -DCMAKE_C_COMPILER=clang" ..
        " -DCMAKE_CXX_COMPILER=clang++"

    if IS_WINDOWS then
        configure_cmd = configure_cmd .. ' -G "Ninja"'
        -- Optional: set a QT_DIR environment variable pointing at your Qt6
        -- kit folder (e.g. C:\Qt\6.8.0\mingw_64) and this picks it up automatically.
        local qt_dir = os.getenv("QT_DIR")
        if qt_dir then
            configure_cmd = configure_cmd .. ' -DCMAKE_PREFIX_PATH="' .. qt_dir .. '"'
        end
    end

    if not run(configure_cmd) then
        print("[ERROR] configure failed")
        os.exit(1)
    end

    print("[BUILD]")
    if not run("cmake --build " .. BUILD_DIR .. " -j" .. tostring(nproc())) then
        print("[ERROR] build failed")
        os.exit(1)
    end

    print("[OK] build complete")
end

local arg1 = arg[1]

if arg1 == "debug" then
    BUILD_TYPE = "Debug"
    build()
    os.exit(0)
end

if arg1 == "clean" then
    print("[CLEAN]")
    if IS_WINDOWS then
        os.execute('rmdir /s /q "' .. BUILD_DIR .. '"')
    else
        os.execute("rm -rf " .. BUILD_DIR)
    end
    print("[OK] clean complete")
    os.exit(0)
end

if arg1 == "run" then
    build()
    local prog = program_path()
    local f = io.open(prog, "r")
    if f then
        f:close()
        print("[RUN] " .. prog)
        os.execute(IS_WINDOWS and ('"' .. prog .. '"') or prog)
    else
        print("[ERROR] executable not found at " .. prog)
        os.exit(1)
    end
    os.exit(0)
end

build()
