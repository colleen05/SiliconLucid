#define TSF_IMPLEMENTATION
#define TML_IMPLEMENTATION
#include <tsf.h>
#include <tml.h>

#include <iostream>
#include <SiliconLucid.hpp>

int main() {
    tsf* TinySoundFont = tsf_load_filename("./resources/soundfonts/a320_neo.sf2");
    tsf_set_output(TinySoundFont, TSF_MONO, 44100, 0); //sample rate
    tsf_note_on(TinySoundFont, 96, 60, 1.0f); //preset 0, middle C
    short RenderedSound[44100*2]; //synthesize 1.0 seconds
    tsf_render_short(TinySoundFont, RenderedSound, 44100*2, 0);
    tsf_close(TinySoundFont);

    InitWindow(320 * 4, 240 * 4, "SILICON LUCID");
    InitAudioDevice();

    Wave wav = {
        44100*2,
        44100,
        16,
        1,
        &RenderedSound
    };

    Sound snd = LoadSoundFromWave(wav);

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(C_DKMAGENTA);

        if(IsKeyPressed(KEY_SPACE)) {
            SetSoundPitch(snd, 0.25f + ((float)GetRandomValue(0, 100) / 100.0f));
            PlaySound(snd);
        }

        EndDrawing();
    }

    UnloadSound(snd);
    UnloadWave(wav);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}