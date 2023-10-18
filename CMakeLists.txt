cmake_minimum_required(VERSION 3.16)

project(
    SiliconLucid
    VERSION 1.0.0
    DESCRIPTION "Silicon Lucid game for LSDJAM 2023."
    HOMEPAGE_URL "https://collydev.itch.io/silicon-lucid"
)

set(CMAKE_CXX_STANDARD 17)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "-gdwarf-4")
endif()

set(SILICONLUCID_INCLUDE_DIR "./include")
file(GLOB_RECURSE SILICONLUCID_SOURCE_FILES "./src/*.cpp")

add_subdirectory("./external/Slew")

add_executable(SiliconLucid ${SILICONLUCID_SOURCE_FILES})

target_include_directories(
    SiliconLucid
    PUBLIC ${SILICONLUCID_INCLUDE_DIR}
)

target_link_libraries(SiliconLucid Slew)
