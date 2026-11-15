// nn::atk unit 2772: updates every voice of a voice list and brings its audio voice's play state in line with the
// requested one. Names and layouts are placeholders.
#include <nn/audio.h>

#include "types.h"

struct UnkAtkVoice;
struct UnkAtkWaveVoice_b0268;

struct UnkAtkVoiceState_l230030 {
    u8 pad_0x0[0x20];
    u8 voice[0x8];  // 0x20, nn::audio::VoiceType
    u8 pad_0x28[0x80];
    u32 state;  // 0xa8
};

struct UnkAtkVoiceList_l230030 {
    u8 pad_0x0[0x8];
    UnkAtkVoiceState_l230030** voices;  // 0x8
    s32 count;                          // 0x10
};

void fn_710122f79c(UnkAtkVoice* self, u32 arg);
void fn_710122f8a8(UnkAtkWaveVoice_b0268* self, u32 arg);

void fn_7101230030(UnkAtkVoiceList_l230030* self, u32 arg) {
    for (s32 i = 0; i < self->count; ++i) {
        fn_710122f79c(reinterpret_cast<UnkAtkVoice*>(self->voices[i]), arg);
        UnkAtkVoiceState_l230030* v = self->voices[i];
        nn::audio::VoiceType* voice = reinterpret_cast<nn::audio::VoiceType*>(v->voice);
        if (!nn::audio::IsVoiceValid(voice))
            continue;
        nn::audio::VoiceType::PlayState play_state = nn::audio::GetVoicePlayState(voice);
        switch (v->state) {
        case 0:
            if (play_state != nn::audio::VoiceType::PlayState_Play)
                fn_710122f8a8(reinterpret_cast<UnkAtkWaveVoice_b0268*>(v), arg);
            break;
        case 1:
            if (play_state == nn::audio::VoiceType::PlayState_Play)
                nn::audio::SetVoicePlayState(voice, nn::audio::VoiceType::PlayState_Stop);
            break;
        case 2:
            if (play_state == nn::audio::VoiceType::PlayState_Play)
                nn::audio::SetVoicePlayState(voice, nn::audio::VoiceType::PlayState_Pause);
            break;
        }
    }
}
