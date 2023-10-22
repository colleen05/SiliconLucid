# Slew
Slew is a C++ game library kit ready for game / game engine development.

## The Tech Stack
| Role | Library | License |
|------|---------|---------|
| Window, rendering, input, models, etc... | [Raylib][raylib] | [Zlib][raylib-license] |
| Rendering | [Raylib][raylib], [Blend2D][blend2d] | [Zlib][raylib-license], [Zlib][blend2d-license] |
| Serialisation | [Alpaca][alpaca] | [MIT][alpaca-license] |
| Entities | [EnTT][entt] | [MIT][entt-license] |
| Input System | [GalaEngine::InputManager][galainput] | [Zlib][galainput-license] |
| Audio | [Raylib][raylib] | [Zlib][raylib-license] |
| GUI | [Dear ImGui][dear-imgui], [rlImGui][rlimgui] | [MIT][dear-imgui-license], [Zlib][rlimgui-license] |
| Scripting | [Lua][lua] | [MIT][lua-license] |

## Building
### Prerequisites
- A C++17 compiler (MSVC, Clang, GCC, etc..)
- CMake
- Ninja (Windows and macOS only)

### Examples
To build examples, simply use `cmake -DBUILD_EXAMPLES=ON ..`. The examples should be located in the `examples` subdirectory of the build directory (e.g. `build/examples/test_imgui[.exe]`).

### Linux
Building on Linux is easy; just use the following commands:
```
git clone https://github.com/colleen05/Slew
cd Slew
mkdir build && cd build
cmake ..
make 
```

### Windows and macOS
Building with CMake for Visual Studio or Xcode can be a little more involved or redundant, so using the [Ninja](https://ninja-build.org/) build system can make the process a little easier. If you already know how to use CMake with your IDE, feel free to use whichever generator you please. The process will be much the same, just without Ninja.

1. If you do not have Ninja installed, do so now. Instructions can be found under the "Getting Ninja" section on the website linked above.
2. Use the following commands:
    ```
    git clone https://github.com/colleen05/Slew
    cd Slew
    mkdir build && cd build
    cmake -G Ninja ..
    ninja
    ```

[raylib]:               https://www.raylib.com/
[raylib-license]:       https://www.raylib.com/license.html
[blend2d]:              https://blend2d.com/
[blend2d-license]:      https://raw.githubusercontent.com/blend2d/blend2d/master/LICENSE.md
[alpaca]:               https://github.com/p-ranav/alpaca
[alpaca-license]:       https://raw.githubusercontent.com/p-ranav/alpaca/master/LICENSE
[entt]:                 https://github.com/skypjack/entt
[entt-license]:         https://raw.githubusercontent.com/skypjack/entt/master/LICENSE
[galainput]:            https://galaengine.com/
[galainput-license]:    https://raw.githubusercontent.com/colleen05/GalaEngine/master/LICENSE
<!-- [soloud]:               https://solhsa.com/soloud/index.html -->
<!-- [soloud-license]:       https://solhsa.com/soloud/legal.html -->
[dear-imgui]:           https://www.dearimgui.com/
[dear-imgui-license]:   https://www.dearimgui.com/licenses/
[rlimgui]:              https://github.com/raylib-extras/rlImGui
[rlimgui-license]:      https://raw.githubusercontent.com/raylib-extras/rlImGui/main/LICENSE
[lua]:                  https://www.lua.org/
[lua-license]:          https://www.lua.org/license.html