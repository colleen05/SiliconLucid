# This file was created specifically for Silicon Lucid, and is not part of
# TinySoundFont. Regardless, this file is still under TinySoundFont's original
# license, as others may wish to integrate it into their own CMake project.
#
# - Colleen ("colleen05")
#
# License: MIT

cmake_minimum_required(VERSION 3.16)
project(
    TinySoundFont
    VERSION 1.0.0
    DESCRIPTION "SoundFont2 synthesizer library in a single C/C++ file."
    HOMEPAGE_URL "https://github.com/schellingb/TinySoundFont"
)

add_library(TinySoundFont STATIC "tsf.h" "tml.h")
set_target_properties(TinySoundFont PROPERTIES LINKER_LANGUAGE C)

target_include_directories(
    TinySoundFont
    PUBLIC "."
)
