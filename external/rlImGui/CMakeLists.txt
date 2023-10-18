# Project: Slew
# File: external/rlImGui/CMakeLists.txt
# Description: rlImGui CMake build configuration.
# License: MIT
# https://github.com/colleen05/Slew

project(rlImGui)

add_library(
    rlImGui STATIC
    "rlImGui.cpp"
)

target_link_libraries(
    rlImGui
    PRIVATE ImGui
    PRIVATE raylib
)

target_include_directories(
    rlImGui
    PUBLIC "."
)
