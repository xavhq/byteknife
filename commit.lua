#!/usr/bin/env lua
-- commit.lua — quick commit + push helper
--
-- Usage:
--   ./commit.lua "commit message"

local function run(cmd)
    print("> " .. cmd)
    local ok = os.execute(cmd)
    return ok == true or ok == 0
end

local msg = arg[1]

if not msg then
    print('Usage: ./commit.lua "commit message"')
    os.exit(1)
end

-- stage everything
if not run("git add -A") then
    print("git add failed, aborting.")
    os.exit(1)
end

-- commit (don't hard-abort if there's nothing to commit)
local escaped_msg = msg:gsub('"', '\\"')
local commit_ok = os.execute('git commit -m "' .. escaped_msg .. '"')
if not (commit_ok == true or commit_ok == 0) then
    print("Nothing to commit, or commit failed — continuing to push anyway.")
end

-- push
if not run("git push") then
    print("git push failed. Check your remote/auth and try again.")
    os.exit(1)
end

print("\nDone.")
