#include <iostream>
#include <SiliconLucid.hpp>

int main() {
    InitWindow(320 * 4, 240 * 4, "SILICON LUCID");

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(C_DKMAGENTA);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}