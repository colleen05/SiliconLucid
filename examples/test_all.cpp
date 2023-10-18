// Project: Slew
// File: examples/test_all.cpp
// Description: TEST example for all dependencies at once.
// License: MIT
// https://github.com/colleen05/Slew

#include <slew/slew.hpp>
#include <alpaca/alpaca.h>
#include <blend2d.h>
#include <entt/entt.hpp>
#include <rlImGui.h>
#include <lua.hpp>

#include <iostream>
#include <algorithm>
#include <vector>
#include <filesystem>
#include <fstream>

#define MENU_ITEM_BOOL(b, s) (b ? "[x] " s : "[ ] " s)

// Core functions
void app_setup();
void app_shutdown();

void setup_scene();
void update_scene();
void draw_scene();
void destroy_scene();

void draw_menubar();
void draw_info_window();
void draw_gui();

bool showDemoWindow = false;
bool isDemoWindowOpen = true;

// Blend2D
Texture load_texture_blend2d(BLImage &image);
void render_about_window_icon();

// Entity components
struct TransformComponent {
    Vector3 position;
    Vector3 scale;
    Vector3 rotation;
};

struct ModelRendererComponent {
    Model model;
    Color colour;
};

Model sphereModel;

Camera camera;
float cam_angle = 0.0f;
float cam_height = 1.75f;
float cam_distance = 4.0f;

entt::registry registry;
std::map<std::string, entt::entity> ent_refs;

struct scene_save_data {
    enum class ent_typeid {
        none,
        ball
    };

    struct ent_data {
        ent_typeid type;
        std::string name;
        std::map<std::string, float> properties;
    };

    std::vector<ent_data> entities;
};

std::vector<std::string> ent_typenames = {
    "unknown-type", "ball"
};

scene_save_data get_scene_data();
void load_scene_data(const scene_save_data &data, bool clear = true);
void save_scene(const std::string &fname);
void load_scene(const std::string &fname, bool clear = true);

void destroy_entity(const std::string &name);
void destroy_all_entities();
void make_ball(float x, float y, float z, float size, float hue, const std::string &name = "");

// Lua & console
lua_State *L;

bool showConsole = true;
bool isConsoleOpen = true;
bool doConsoleArgDump = false;
char *consoleInputBuffer;

struct console_entry {
    int type; // 0 = output; 1 = error; 2 = user input.
    std::string text;
};

std::vector<console_entry> consoleLog;
int LUA_print(lua_State *L);
int LUA_console(lua_State *L);
int LUA_destroy_ent(lua_State *L);
int LUA_clear_ents(lua_State *L);
int LUA_ent_create_ball(lua_State *L);
void execute_console_command(const std::string &command);
void draw_console_window();

// ImGui stuff, misc.
bool showAboutWindow = false;
bool isAboutWindowOpen = true;
Texture aboutWindowIcon;
void draw_about_window();

void SetupImGuiStyle();

int main() {
    slew::print_lib_info();
    std::cout << std::endl;

    if(std::filesystem::exists("imgui.ini")) {
        std::filesystem::remove("imgui.ini");
    }

    app_setup();

    while(!WindowShouldClose()) {
        // Logic
        update_scene();

        // Drawing
        BeginDrawing();
        
        ClearBackground(BLACK);
        draw_scene();

        rlImGuiBegin();
        draw_gui();
        rlImGuiEnd();

        EndDrawing();
    }

    app_shutdown();

    return 0;
}

void app_setup() {
    // Window, audio, and GUI
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(1600, 900, "[Slew] Test Application");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    InitAudioDevice();
    
    rlImGuiSetup(true);

    // Lua & console
    L = luaL_newstate();
    
    lua_pushcfunction(L, luaopen_base);
    lua_call(L, 0, 0);
    lua_pushcfunction(L, luaopen_math);
    lua_call(L, 0, 0);
    lua_pushcfunction(L, luaopen_string);
    lua_call(L, 0, 0);
    lua_pushcfunction(L, luaopen_table);
    lua_call(L, 0, 0);

    luaL_requiref(L, "math", luaopen_math, true);

    lua_pushcfunction(L, LUA_print);
    lua_setglobal(L, "print");
    lua_pushcfunction(L, LUA_console);
    lua_setglobal(L, "console");
    lua_pushcfunction(L, LUA_ent_create_ball);
    lua_setglobal(L, "ent_create_ball");
    lua_pushcfunction(L, LUA_destroy_ent);
    lua_setglobal(L, "ent_destroy");
    lua_pushcfunction(L, LUA_clear_ents);
    lua_setglobal(L, "ents_clear");

    consoleInputBuffer = new char[1024]{0};

    // Camera
    camera.position = { 0.0f, 0.0f, 0.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 75.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Resources
    sphereModel = LoadModelFromMesh(GenMeshSphere(1.0f, 32, 24));

    render_about_window_icon();

    // GUI
    SetupImGuiStyle();
}

void app_shutdown() {
    // Lua
    lua_close(L);

    // Resources
    UnloadModel(sphereModel);
    UnloadTexture(aboutWindowIcon);

    // Window, audio, and GUI
    rlImGuiShutdown();
    CloseAudioDevice();
    CloseWindow();
}

void update_scene() {
    cam_angle += 15.0f * GetFrameTime();

    camera.position = {
        std::sin(cam_angle * DEG2RAD) * cam_distance,
        cam_height,
        std::cos(cam_angle * DEG2RAD) * cam_distance
    };
}

void draw_scene() {
    BeginMode3D(camera);

    DrawGrid(16, 1.0f);

    auto renderingView = registry.view<ModelRendererComponent, TransformComponent>();

    for(auto[ent, model, transform] : renderingView.each()) {
        DrawModelEx(model.model, transform.position, {}, 0.0f, transform.scale, model.colour);
    }

    EndMode3D();
}

void draw_menubar() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::BeginMainMenuBar();
        if(ImGui::BeginMenu("File")) {
            ImGui::BeginPopup("MenuBar.File");
                if(ImGui::MenuItem("Save scene")) {
                    save_scene("scene.sav");
                }
                if(ImGui::MenuItem("Load scene")) {
                    load_scene("scene.sav");
                }
                ImGui::Separator();
                if(ImGui::MenuItem("Exit")) {
                    load_scene("scene.sav");
                }
            ImGui::EndPopup();
        }

        if(ImGui::BeginMenu("View")) {
            ImGui::BeginPopup("MenuBar.View");
                if(ImGui::MenuItem(MENU_ITEM_BOOL(showConsole, "Console"))) {
                    showConsole = !showConsole;
                }
                ImGui::Separator();
                if(ImGui::MenuItem(MENU_ITEM_BOOL(showDemoWindow, "ImGui Demo Window"))) {
                    showDemoWindow = !showDemoWindow;
                }
            ImGui::EndPopup();
        }

        if(ImGui::BeginMenu("Help")) {
            ImGui::BeginPopup("MenuBar.Help");
                if(ImGui::MenuItem(MENU_ITEM_BOOL(showAboutWindow, "About"))) {
                    showAboutWindow = !showAboutWindow;
                }
            ImGui::EndPopup();
        }
    ImGui::EndMainMenuBar();
    ImGui::PopStyleVar();
}

void draw_info_window() {
    ImGui::PushStyleColor(ImGuiCol_WindowBg, {0.0f, 0.0f, 0.0f, 0.75f});
    ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::SetWindowPos({8.0f, 28.0f});
        ImGui::Text("FPS: %d | Entities: %lu", GetFPS(), registry.storage<entt::entity>().size());
        ImGui::Separator();
        ImGui::Text("Camera Mode:     Orbit");
        ImGui::Text("Camera Position: <%f, %f, %f>", camera.position.x, camera.position.y, camera.position.z);
    ImGui::End();
    ImGui::PopStyleColor();
}

void draw_gui() {
    draw_menubar();

    if(showConsole) draw_console_window();
    if(showDemoWindow) ImGui::ShowDemoWindow(&isDemoWindowOpen);
    if(showAboutWindow) draw_about_window();

    draw_info_window();
}

// Blend2D stuff
Texture load_texture_blend2d(BLImage &image) {
    BLImageData imgData;
    image.getData(&imgData);

    // Prepare image
    Image img = {
        .data = nullptr,
        .width = imgData.size.w,
        .height = imgData.size.h,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
    };

    // Convert
    BLFormatInfo srcInfo = blFormatInfo[imgData.format];

    BLFormatInfo dstInfo;
    dstInfo.depth = 32;
    dstInfo.flags = BL_FORMAT_FLAG_RGBA;
    dstInfo.setSizes(8, 8, 8, 8);
    dstInfo.setShifts(0, 8, 16, 24);

    BLPixelConverter cvt;
    cvt.create(dstInfo, blFormatInfo[imgData.format]);

    uint8_t* dstData = (uint8_t*)(malloc(imgData.size.w * imgData.size.h * 4));
    cvt.convertRect(dstData, imgData.size.w * 4, imgData.pixelData, imgData.stride, imgData.size.w, imgData.size.h);

    img.data = dstData;

    // Set
    Texture tex = LoadTextureFromImage(img);
    SetTextureFilter(tex, TEXTURE_FILTER_BILINEAR);
    UnloadImage(img);

    return tex;
}

void render_about_window_icon() {
    // Render
    BLImage img(128, 128, BL_FORMAT_PRGB32);
    BLContext ctx(img);

    ctx.setCompOp(BL_COMP_OP_SRC_COPY);
    ctx.setFillStyle(BLRgba32(0x00000000));
    ctx.fillAll();

    BLGradient grad_blue(BLLinearGradientValues(0, 0, 0, 128));
    grad_blue.addStop(0.0, BLRgba32(0xFF3FC370));
    grad_blue.addStop(1.0, BLRgba32(0xFF0F7F4F));

    BLGradient grad_white(BLLinearGradientValues(0, 0, 0, 128));
    grad_white.addStop(0.0, BLRgba32(0xFFFFFFFF));
    grad_white.addStop(1.0, BLRgba32(0xFFAAAAAA));

    BLPath path1;
    path1.moveTo(64, 32);
    path1.lineTo(64, 76);
    
    ctx.setCompOp(BL_COMP_OP_SRC_OVER);

    ctx.setFillStyle(grad_white);
    ctx.fillCircle(64, 64, 64);
    ctx.setFillStyle(grad_blue);
    ctx.fillCircle(64, 64, 60);

    ctx.setStrokeStyle(grad_white);
    ctx.setStrokeWidth(8);
    ctx.setStrokeStartCap(BL_STROKE_CAP_ROUND);
    ctx.setStrokeEndCap(BL_STROKE_CAP_ROUND);
    ctx.strokePath(path1);

    ctx.setFillStyle(grad_white);
    ctx.fillCircle(64, 94, 4);

    ctx.end();

    // Load to texture
    aboutWindowIcon = load_texture_blend2d(img);
}

// Entity stuff
scene_save_data get_scene_data() {
    scene_save_data data;

    auto &entStorage = registry.storage<entt::entity>();
    auto view = registry.view<TransformComponent, ModelRendererComponent>();

    for(const auto &[ent, transform, renderer] : view.each()) {
        scene_save_data::ent_data entData {};
        
        std::string eref_name = "";

        for(const auto &[name, e] : ent_refs)
            if(e == ent) eref_name = name;

        entData.name = eref_name;
        entData.properties = {
            {"x", transform.position.x},
            {"y", transform.position.y},
            {"z", transform.position.z},
            {"scale", transform.scale.x},
            {"hue", ColorToHSV(renderer.colour).x}
        };

        entData.type = scene_save_data::ent_typeid::ball;

        data.entities.push_back(entData);
    }

    return data;
}

void load_scene_data(const scene_save_data &data, bool clear) {
    if(clear) destroy_all_entities();

    for(const auto &e : data.entities) {
        bool doLog = true;
        Vector3 log_entPos = {0.0f, 0.0f, 0.0f};

        if(e.type == scene_save_data::ent_typeid::none) {
            consoleLog.push_back({
                1,
                "Attempted to load null-type entity" + std::string(
                    e.name.empty() ?
                    "." : (": \"" + e.name + "\".")
                )
            });
            doLog = false;
        }else if(e.type == scene_save_data::ent_typeid::ball) {
            const float x       = (e.properties.find("x") != e.properties.end()) ? e.properties.at("x") : 0.0f;
            const float y       = (e.properties.find("y") != e.properties.end()) ? e.properties.at("y") : 0.0f;
            const float z       = (e.properties.find("z") != e.properties.end()) ? e.properties.at("z") : 0.0f;
            const float scale   = (e.properties.find("scale") != e.properties.end()) ? e.properties.at("scale") : 1.0f;
            const float hue     = (e.properties.find("hue") != e.properties.end()) ? e.properties.at("hue") : 0.0f;
            make_ball(x, y, z, scale, hue, e.name);
            log_entPos = {x, y, z};
        }else {
            consoleLog.push_back({
                1,
                "Attempted to load unknown-type (" + std::to_string((int)e.type) + ") entity" + std::string(
                    e.name.empty() ?
                    "." : (": \"" + e.name + "\".")
                )
            });
            doLog = false;
        }
    }
}

void save_scene(const std::string &fname) {
    auto save_data = get_scene_data();

    consoleLog.push_back({0, "Saving scene to \"" + fname + "\"..."});

    std::vector<uint8_t> bytes;

    try {
        alpaca::serialize(save_data, bytes);
        std::ofstream f(fname, std::ios::binary);
        f.write((char*)&bytes[0], bytes.size());
        f.close();
    }catch(const std::exception &e) {
        consoleLog.push_back({1, "Uncaught error."});
        return;
    }

    consoleLog.push_back({0, "Done. Wrote " + std::to_string(bytes.size()) + " byte(s)."});
}

void load_scene(const std::string &fname, bool clear) {
    if(!std::filesystem::exists(fname) || std::filesystem::is_directory(fname)) {
        consoleLog.push_back({1, "File \"" + fname + "\" does not exist."});
        return;
    }

    consoleLog.push_back({0, "Loading scene from \"" + fname + "\"."});

    std::vector<uint8_t> bytes;
    std::ifstream f(fname, std::ios::binary);
    bytes = std::vector<uint8_t>(
        std::istreambuf_iterator<char>(f),
        std::istreambuf_iterator<char>()
    );
    f.close();

    scene_save_data save_data;

    std::error_code ec;
    save_data = alpaca::deserialize<scene_save_data>(bytes, ec);

    if(ec) {
        consoleLog.push_back({1, "ERROR " + std::to_string(ec.value()) + ": " + ec.message()});
        return;
    }

    load_scene_data(save_data);
}

void destroy_entity(const std::string &name) {
    auto ent = ent_refs.find(name);

    if(ent == ent_refs.end()) {
        consoleLog.push_back({1, "Attempted to destroy non-existent entity: \"" + name + "\"."});
        return;
    }

    registry.destroy(ent->second);
    ent_refs.erase(ent->first);

    consoleLog.push_back({0, "Destroyed entity: \"" + name + "\"."});
}

void destroy_all_entities() {
    const auto entCount = registry.storage<entt::entity>().size();

    registry.clear();
    registry.storage<entt::entity>().clear();
    ent_refs.clear();
    consoleLog.push_back({0, "Destroyed all " + std::to_string(entCount) + " entities."});
}

void make_ball(float x, float y, float z, float size, float hue, const std::string &name) {
    if(ent_refs.find(name) != ent_refs.end()) {
        consoleLog.push_back({1, "Entity with name \"" + name + "\" already exists."});
        return;
    }

    entt::entity ent = registry.create();
    registry.emplace<TransformComponent>(
        ent,
        Vector3 {x, y, z},
        Vector3 {size, size, size},
        Vector3 {0.0f, 0.0f, 0.0f}
    );
    registry.emplace<ModelRendererComponent>(
        ent,
        sphereModel,
        ColorFromHSV(hue, 0.75f, 1.0f)
    );

    const std::string &refName = name.empty() ? ("#" + std::to_string((uint32_t)ent)) : name;
    ent_refs.insert(std::make_pair<>(refName, ent));

    consoleLog.push_back({
        0,
        "Created ball entity \"" + refName + "\" at <" +
        std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ">."
    });
}

// Lua console
#pragma region
int LUA_print(lua_State *L) {
    const char *s = lua_tostring(L, -1);
    consoleLog.push_back({0, s});
    lua_pop(L, 1);
    return 0;
}

int LUA_console(lua_State *L) {
    const char *s = lua_tostring(L, -1);
    execute_console_command(s);
    lua_pop(L, 1);
    return 0;
}

int LUA_destroy_ent(lua_State *L) {
    const char *s = lua_tostring(L, -1);
    destroy_entity(s);
    lua_pop(L, 1);
    return 0;
}

int LUA_clear_ents(lua_State *L) {
    destroy_all_entities();
    return 0;
}

int LUA_ent_create_ball(lua_State *L) {
    float x = lua_tonumber(L, -5);
    float y = lua_tonumber(L, -4);
    float z = lua_tonumber(L, -3);
    float s = lua_tonumber(L, -2);
    float hue = lua_tonumber(L, -1);
    make_ball(x, y, z, s, hue);
    lua_pop(L, 4);
    return 0;
}

bool consoleLogScrollDown = false;
void draw_console_window() {
    ImGui::Begin("Console", &showConsole, ImGuiWindowFlags_NoResize);
        ImGui::Text("TIP: Type \"help\" for help!");

        ImGui::BeginChild("Console.Log", {600, 300}, true, ImGuiWindowFlags_HorizontalScrollbar);
            for(auto &s : consoleLog) {
                ImVec4 textColour;
                switch(s.type) {
                    default:
                    case 0: textColour = {0.6f, 0.6f, 0.6f, 1.0f}; break;
                    case 1: textColour = {1.0f, 0.25f, 0.25f, 1.0f}; break;
                    case 2: textColour = {1.0f, 1.0f, 1.0f, 1.0f}; break;
                }
                ImGui::PushStyleColor(ImGuiCol_Text, textColour);
                ImGui::Text("%s", s.text.c_str());
                ImGui::PopStyleColor();
            }

            if(consoleLogScrollDown) {
                ImGui::SetScrollHereY(1.0f);
                consoleLogScrollDown = false;
            }
        ImGui::EndChild();

        if(ImGui::InputText(" ", consoleInputBuffer, 1024, ImGuiInputTextFlags_EnterReturnsTrue)) {
            if(!std::string(consoleInputBuffer).empty()) {
                consoleLog.push_back({2, "> " + std::string(consoleInputBuffer)});
                execute_console_command(consoleInputBuffer);
                memset(consoleInputBuffer, 0, strlen(consoleInputBuffer));
                consoleLogScrollDown = true;
            }

            ImGui::SetItemDefaultFocus();
            ImGui::SetKeyboardFocusHere(-1);
        }

        ImGui::SameLine();
        ImGui::Button("Execute");
    ImGui::End();
}

std::vector<std::string> split_command(const std::string &command) {
    enum class splitter_state {
        awaiting_argument,
        in_argument,
        in_string,
        in_escape
    };

    std::vector<splitter_state> state_stack {splitter_state::awaiting_argument};
    std::vector<std::string> out;
    std::string buffer;

    for(const char c : command) {
        const auto state = state_stack[state_stack.size() - 1];
        switch(state) {
            case splitter_state::awaiting_argument:
                state_stack.push_back(splitter_state::in_argument);
                switch(c) {
                    default: buffer += c; break;
                    case '\\': state_stack.push_back(splitter_state::in_escape); break;
                    case '\"': state_stack.push_back(splitter_state::in_string); break;
                    case ' ': state_stack.pop_back(); break;
                }
                break;
            case splitter_state::in_argument:
                switch(c) {
                    default: buffer += c; break;
                    case '\\': state_stack.push_back(splitter_state::in_escape); break;
                    case '\"': state_stack.push_back(splitter_state::in_string); break;
                    case ' ':
                        out.push_back(buffer);
                        buffer.clear();
                        state_stack.pop_back();
                        break;
                }
                break;
            case splitter_state::in_string:
                switch(c) {
                    default: buffer += c; break;
                    case '\\': state_stack.push_back(splitter_state::in_escape); break;
                    case '\"': state_stack.pop_back(); break;
                }
                break;
            case splitter_state::in_escape:
                buffer += c;
                state_stack.pop_back();
                break;
        }
    }

    if(!buffer.empty()) out.push_back(buffer);
    return out;
}

void execute_console_command(const std::string &command) {
    std::vector<std::string> args = split_command(command);

    if(args.empty()) return;
    if(args[0][0] == '#') return;

    if((args[0] != "argdump") && doConsoleArgDump) {
        for(int i = 0; i < args.size(); i++)
            consoleLog.push_back({0, std::to_string(i) + ": " + args[i] + "\n"});

        return;
    }

    if(args[0] == "help") {
        consoleLog.push_back({0,
            "Commands:\n"\
            "  - help                    Show this help message.\n"\
            "  - echo <string>           Echo back string.\n"
            "  - clear                   Clear log.\n"\
            "  - argdump [bool]          Sets or displays argument dumping.\n"\
            "  - ent_create_ball         Makes a ball entity at position, with size and colour.\n"\
            "    <x> <y> <z> <size> <hue>\n"\
            "  - ent_destroy <name...>   Destroy entities by name.\n"\
            "  - ents_clear              Destroy all entities.\n"\
            "  - ents_list               List all entities.\n"\
            "  - type <file>             Print contents of a file.\n"\
            "  - exec <file>             Execute file as console script.\n"
            "  - lua_exec <code>         Execute lua code.\n"\
            "  - lua_execf <file>        Execute lua file.\n"\
            "  - save [file]             Save scene.\n"\
            "  - load [file]             Load scene.\n"\
        });
    }else if(args[0] == "echo") {
        if(args.size() < 2) return;
        consoleLog.push_back({0, args[1]});
    }else if(args[0] == "clear") {
        consoleLog.clear();
    }else if(args[0] == "type") {
        if(args.size() == 1 || std::filesystem::is_directory(args[1])) {
            consoleLog.push_back({1, "Please specify a file."});
            return;
        }

        std::ifstream f(args[1]);
        
        if(!f.good()) {
            consoleLog.push_back({1, "Bad file."});
            return;
        }

        consoleLog.push_back({0, std::string(
            std::istreambuf_iterator<char>(f),
            std::istreambuf_iterator<char>()
        )});
    }else if(args[0] == "argdump") {
        if(args.size() == 1) {
            consoleLog.push_back({0, "doConsoleArgDump = " + std::string(doConsoleArgDump ? "true" : "false") + "."});
        }else if(args[1] == "true" || args[1] == "1") {
            doConsoleArgDump = true;
            consoleLog.push_back({0, "Enabled argument dumping."});
        }else if(args[1] == "false" || args[1] == "0") {
            doConsoleArgDump = false;
            consoleLog.push_back({0, "Disabled argument dumping."});
        }else {
            consoleLog.push_back({1, "Please specify true, false, 1, or 0."});
        }
    }else if(args[0] == "ent_create_ball") {
        if(args.size() < 6) {
            consoleLog.push_back({1, "Please specify coordinates, size, and hue."});
            return;
        }

        make_ball(
            std::atof(args[1].c_str()), std::atof(args[2].c_str()), std::atof(args[3].c_str()),
            std::atof(args[4].c_str()), std::atof(args[5].c_str())
        );
    }else if(args[0] == "ent_destroy") {
        if(args.size() < 2) {
            consoleLog.push_back({1, "Please specify 1 or more entities."});
            return;
        }

        for(int i = 0; i < args.size() - 1; i++)
            destroy_entity(args[i + 1]);
    }else if(args[0] == "ents_clear") {
        destroy_all_entities();
    }else if(args[0] == "ents_list") {
        auto &entStorage = registry.storage<entt::entity>();

        if(!entStorage.in_use()) {
            consoleLog.push_back({0, "No entities in scene."});
            return;
        }

        consoleLog.push_back({0, std::to_string(entStorage.size()) + " entities in scene:"});

        for(const auto &[ent] : entStorage.each()) {
            std::string eref_name = "";

            for(const auto &[name, e] : ent_refs)
                if(e == ent) eref_name = name;

            consoleLog.push_back({
                0,
                "  [" + std::to_string((uint32_t)ent) + "]" +
                    std::string((!eref_name.empty()) ? (" " + eref_name) : "")
            });
        }
    }else if(args[0] == "exec") {
        if(args.size() == 1 || std::filesystem::is_directory(args[1])) {
            consoleLog.push_back({1, "Please specify a file."});
            return;
        }

        std::ifstream f(args[1]);
        
        if(!f.good()) {
            consoleLog.push_back({1, "Bad file."});
            return;
        }

        for(std::string line; std::getline(f, line);)
            execute_console_command(line);
    } else if(args[0] == "lua_exec") {
        std::string code = command.substr(args[0].length());
        try {
            luaL_dostring(L, code.c_str());
        } catch (std::exception &e) {
            consoleLog.push_back({1, "Execution error."});
        }

    }else if(args[0] == "lua_execf") {
        if(args.size() == 1 || std::filesystem::is_directory(args[1])) {
            consoleLog.push_back({1, "Please specify a file."});
            return;
        }

        std::ifstream f(args[1]);
        
        if(!f.good()) {
            consoleLog.push_back({1, "Bad file."});
            return;
        }

        try {
            luaL_dofile(L, args[1].c_str());
        } catch (std::exception &e) {
            consoleLog.push_back({1, "Execution error."});
        }
    }else if(args[0] == "save") {
        const std::string fname = (args.size() >= 2) ? args[1] : "scene.sav";
        save_scene(fname);
    }else if(args[0] == "load") {
        const std::string fname = (args.size() >= 2) ? args[1] : "scene.sav";
        load_scene(fname);
    }else {
        auto args = split_command(command);
        consoleLog.push_back({1, "Unknown command."});
    }
}
#pragma endregion

// ImGui, misc
void draw_about_window() {
    ImGui::Begin("About Slew", &showAboutWindow, ImGuiWindowFlags_NoResize);
        ImGui::Image(&aboutWindowIcon, {96, 96});
        if(ImGui::IsItemHovered()) {
           ImGui::SetTooltip("Rendered with Blend2D."); 
        }

        ImGui::SameLine();
        ImGui::PushTextWrapPos(512);
        ImGui::Text("\nSlew is a C++ game library kit ready for game / game engine development.\n\nWritten by Colleen (\"colleen05\").\nLicense: MIT");
        ImGui::PopTextWrapPos();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - 80.0f);
        if(ImGui::Button("Learn more", {80.0f, 20.0f})) {
            OpenURL("https://github.com/colleen05/Slew/");
        }
    ImGui::End();
}

void SetupImGuiStyle() {
	// Slew style from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();
	
	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6000000238418579f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 4.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_None;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 2.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4.0f, 3.0f);
	style.FrameRounding = 2.0f;
	style.FrameBorderSize = 1.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 8.0f;
	style.GrabMinSize = 8.0f;
	style.GrabRounding = 2.0f;
	style.TabRounding = 2.0f;
	style.TabBorderSize = 1.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.0f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
	
	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05882352963089943f, 0.05882352963089943f, 0.05882352963089943f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9399999976158142f);
	style.Colors[ImGuiCol_Border] = ImVec4(1.0f, 1.0f, 1.0f, 0.1215686276555061f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.125490203499794f, 0.125490203499794f, 0.125490203499794f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.1882352977991104f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.03921568766236305f, 0.03921568766236305f, 0.03921568766236305f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.05882352963089943f, 0.4980392158031464f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.5099999904632568f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.5299999713897705f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.407843142747879f, 0.407843142747879f, 0.407843142747879f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.2470588237047195f, 0.7647058963775635f, 0.4392156898975372f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.05882352963089943f, 0.4980392158031464f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.2470588237047195f, 0.7647058963775635f, 0.4392156898975372f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.05882352963089943f, 0.4980392158031464f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.2470588237047195f, 0.6235294342041016f, 0.4392156898975372f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.05882352963089943f, 0.4980392158031464f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.05882352963089943f, 0.4980392158031464f, 0.3098039329051971f, 0.5058823823928833f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.05882352963089943f, 0.4980392158031464f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.05882352963089943f, 0.4980392158031464f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.3098039329051971f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.501960813999176f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.05882352963089943f, 0.4980392158031464f, 0.3098039329051971f, 0.3725490272045135f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.05882352963089943f, 0.4980392158031464f, 0.3098039329051971f, 0.5058823823928833f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.05882352963089943f, 0.4980392158031464f, 0.3098039329051971f, 0.9411764740943909f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.05882352963089943f, 0.4980392158031464f, 0.3098039329051971f, 0.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2470588237047195f, 0.6235294342041016f, 0.4392156898975372f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.05882352963089943f, 0.4980392158031464f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0235294122248888f, 0.3098039329051971f, 0.1843137294054031f, 0.7647058963775635f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0235294122248888f, 0.3098039329051971f, 0.1843137294054031f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.2470588237047195f, 0.7647058963775635f, 0.4392156898975372f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.2470588237047195f, 0.6235294342041016f, 0.4392156898975372f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.05882352963089943f, 0.4980392158031464f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.0f, 0.9450980424880981f, 1.0f, 0.2431372553110123f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.2470588237047195f, 0.7647058963775635f, 0.4392156898975372f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2470588237047195f, 0.7647058963775635f, 0.4392156898975372f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.7019608020782471f);
}
