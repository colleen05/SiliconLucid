// Project: Slew
// File: examples/test_raylib.cpp
// Description: TEST example for opening a raylib window.
// License: MIT
// https://github.com/colleen05/Slew

#include <slew/slew.hpp>
#include <raylib.h>

int main() {
    slew::print_lib_info();

    InitWindow(800, 600, "Hello, Window!");

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RED);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}