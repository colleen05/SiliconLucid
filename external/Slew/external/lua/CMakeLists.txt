# Project: Slew
# File: external/lua/CMakeLists.txt
# Description: Lua CMake build configuration.
# License: MIT
# https://github.com/colleen05/Slew

project(lua)

file(GLOB_RECURSE LUA_SOURCE_FILES "*.c")
file(GLOB_RECURSE LUA_INCLUDE_FILES "*.h")

add_library(lua STATIC ${LUA_SOURCE_FILES})

target_include_directories(
    lua
    PUBLIC "."
)
