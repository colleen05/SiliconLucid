#define TSF_IMPLEMENTATION
#define TML_IMPLEMENTATION
#include <tsf.h>
#include <tml.h>

#include <iostream>
#include <SiliconLucid.hpp>

int main() {
    tsf* TinySoundFont = tsf_load_filename("soundfont.sf2");
    tsf_set_output(TinySoundFont, TSF_MONO, 44100, 0); //sample rate
    tsf_note_on(TinySoundFont, 0, 60, 1.0f); //preset 0, middle C
    short HalfSecond[22050]; //synthesize 0.5 seconds
    tsf_render_short(TinySoundFont, HalfSecond, 22050, 0);
    tsf_close(TinySoundFont);

    InitWindow(320 * 4, 240 * 4, "SILICON LUCID");
    InitAudioDevice();

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(C_DKMAGENTA);
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}