# SiliconLucid
Silicon Lucid game for LSDJAM 2023.

## Building
### Prerequisites
- A C++17 compiler (MSVC, Clang, GCC, etc..)
- CMake
- Ninja (Windows and macOS only)

### Linux
Building on Linux is easy; just use the following commands:
```
git clone https://github.com/colleen05/SiliconLucid
cd SiliconLucid
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make 
```

### Windows and macOS
Building with CMake for Visual Studio or Xcode can be a little more involved or redundant, so using the [Ninja](https://ninja-build.org/) build system can make the process a little easier. If you already know how to use CMake with your IDE, feel free to use whichever generator you please. The process will be much the same, just without Ninja.

1. If you do not have Ninja installed, do so now. Instructions can be found under the "Getting Ninja" section on the website linked above.
2. Use the following commands:
    ```
    git clone https://github.com/colleen05/SiliconLucid
    cd SiliconLucid
    mkdir build && cd build
    cmake -G Ninja .. -DCMAKE_BUILD_TYPE=Release
    ninja
    ```

## LICENSE & EXCEPTION NOTICE
All original code written for this project is distributed under the **3-Clause BSD license**, as written in the `LICENSE` file.

Exceptions to this are as follows:
1. Any files in the `external` directory have their own licenses, included in their respective directories.
2. The resources in the `resources` folder are UNDECIDED, and all rights are reserved until an appropriate sharing license is decided upon.