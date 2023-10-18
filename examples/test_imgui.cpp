// Project: Slew
// File: examples/test_imgui.cpp
// Description: TEST example for rlImGui, Dear ImGui for Raylib.
// License: MIT
// https://github.com/colleen05/Slew

#include <slew/slew.hpp>
#include <rlImGui.h>
#include <cmath>

bool isDemoWindowOpen = true;

float cam_angle = 0.0f;
float cam_height = 1.75f;
float cam_distance = 4.0f;

float model_hue = 0.0f;
float model_sat = 1.0f;
float model_val = 1.0f;

Camera camera;
Model model;

void draw_background() {
    // Logic
    cam_angle += 15.0f * GetFrameTime();

    camera.position = {
        std::sin(cam_angle * DEG2RAD) * cam_distance,
        cam_height,
        std::cos(cam_angle * DEG2RAD) * cam_distance
    };

    model_hue += 360.0f * GetFrameTime() / 10.0f;

    // Drawing
    ClearBackground({0x11, 0x11, 0x11, 0xff});
    BeginMode3D(camera);

    DrawGrid(16, 1.0f);
    DrawModel(model, {0.0f, 0.5f, 0.0f}, 1.0f, ColorFromHSV(model_hue, model_sat, model_val));

    EndMode3D();
}

void draw_gui() {
    rlImGuiBegin();
    ImGui::ShowDemoWindow(&isDemoWindowOpen);
    rlImGuiEnd();
}

int main() {
    slew::print_lib_info();

    // Set up window
	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
	InitWindow(1600, 900, "[Slew] Dear ImGui + Raylib (rlImGui)");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    SetWindowState(FLAG_WINDOW_RESIZABLE);
	rlImGuiSetup(true);

    // Set up camera
    camera.position = { 0.0f, 0.0f, 0.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 75.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Generate model texture
    Image img_texture = GenImagePerlinNoise(512, 512, 0.0f, 0.0f, 4.0f);
    ImageColorContrast(&img_texture, -25);
    ImageColorBrightness(&img_texture, 64);
    Texture texture = LoadTextureFromImage(img_texture);
    UnloadImage(img_texture);

    // Set preview model
    model = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
    model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = texture;

    // Enter game loop
	while(!WindowShouldClose()) {
        if(IsKeyPressed(KEY_F11)) {
            ToggleFullscreen();
        }

		BeginDrawing();
        draw_background();
        draw_gui();
		EndDrawing();
	}

    // Shut down
    UnloadTexture(texture);
    UnloadModel(model);

	rlImGuiShutdown(); 
	CloseWindow();

    return 0;
}