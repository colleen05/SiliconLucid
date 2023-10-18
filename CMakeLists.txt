# Project: Slew
# File: CMakeLists.txt
# Description: Slew CMake build configuration.
# License: MIT
# https://github.com/colleen05/Slew

cmake_minimum_required(VERSION 3.16)
project(
    Slew
    VERSION 0.0.1
    DESCRIPTION "A C++ game engine born from a slew of third-party libraries. "
    HOMEPAGE_URL "https://github.com/colleen05/Slew"
)
set(CMAKE_CXX_STANDARD 17)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "-gdwarf-4")
endif()

# set(BUILD_SHARED_LIBS OFF)
# set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# External libraries
set(BLEND2D_STATIC true)

add_subdirectory("./external/raylib")
add_subdirectory("./external/blend2d")
add_subdirectory("./external/alpaca")
add_subdirectory("./external/entt")
add_subdirectory("./external/imgui")
add_subdirectory("./external/rlImGui")
add_subdirectory("./external/lua")

# Slew
set(SLEW_SOURCE_DIR "src")
set(SLEW_INCLUDE_DIR "include")

file(GLOB_RECURSE SLEW_SOURCE_FILES "${SLEW_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE SLEW_INCLUDE_FILES "${SLEW_INCLUDE_DIR}/*.hpp")

add_library(Slew STATIC ${SLEW_SOURCE_FILES})

target_link_libraries(
    Slew
    PUBLIC raylib
    PUBLIC blend2d
    PUBLIC alpaca
    PUBLIC EnTT
    PUBLIC ImGui
    PUBLIC rlImGui
    PUBLIC lua
)

target_include_directories(
    Slew
    PUBLIC ${SLEW_INCLUDE_DIR}
)

# Examples
if(${BUILD_EXAMPLES})
    add_subdirectory("examples")
endif()
