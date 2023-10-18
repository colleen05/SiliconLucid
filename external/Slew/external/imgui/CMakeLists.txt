# Project: Slew
# File: external/imgui/CMakeLists.txt
# Description: Dear ImGui CMake build configuration.
# License: MIT
# https://github.com/colleen05/Slew

project(ImGui)

add_library(
    ImGui STATIC
    "imgui_demo.cpp"
    "imgui_draw.cpp"
    "imgui_tables.cpp"
    "imgui_widgets.cpp"
    "imgui.cpp"
)

target_include_directories(
    ImGui
    PUBLIC "."
)
