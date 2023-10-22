#define TSF_IMPLEMENTATION
#define TML_IMPLEMENTATION
#include <tsf.h>
#include <tml.h>

#include <iostream>
#include <SiliconLucid.hpp>

int main() {
    const int sampleRate = 44100;
    const int soundDuration = 2;
    const int preset = 10;
    const int note = 60;
    const int noteDuration = 1;

    short renderedSound[sampleRate * soundDuration];

    tsf* synth = tsf_load_filename("./resources/soundfonts/a320_neo.sf2");
    tsf_set_output(synth, TSF_MONO, sampleRate, 0);

    tsf_note_on(synth, preset, note, 1.0f);
    tsf_render_short(synth, renderedSound, noteDuration * sampleRate, 0);

    tsf_note_off(synth, preset, note);
    tsf_render_short(synth, renderedSound + (noteDuration * sampleRate), (soundDuration - noteDuration) * sampleRate, 0);

    tsf_close(synth);

    InitWindow(320 * 4, 240 * 4, "SILICON LUCID");
    InitAudioDevice();

    Wave wav = {
        44100*2,
        44100,
        16,
        1,
        &renderedSound
    };

    Sound snd = LoadSoundFromWave(wav);

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(C_DKMAGENTA);

        if(IsKeyPressed(KEY_SPACE)) {
            SetSoundPitch(snd, 0.5f + ((float)GetRandomValue(0, 100) / 100.0f));
            PlaySound(snd);
        }

        EndDrawing();
    }

    UnloadSound(snd);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}