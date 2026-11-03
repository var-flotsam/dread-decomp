// nn::atk (tools/tu_map.py, library unit): stores a voice's priority (+0xa4) and, when its audio voice
// (+0x20) is valid, passes it on inverted (0xff maps to 0). Names and layout are placeholders.
#include <nn/audio.h>

#include "types.h"

struct UnkAtkVoice_b48a3 {
    u8 pad_0x0[0x20];
    u8 voice[0x8];  // nn::audio::VoiceType
    u8 pad_0x28[0xa4 - 0x28];
    s32 priority;
};

static inline nn::audio::VoiceType* Voice(UnkAtkVoice_b48a3* self) {
    return reinterpret_cast<nn::audio::VoiceType*>(self->voice);
}

void fn_710122f5a4(UnkAtkVoice_b48a3* self, s32 priority) {
    self->priority = priority;
    if (nn::audio::IsVoiceValid(Voice(self))) {
        s32 p = self->priority;
        nn::audio::SetVoicePriority(Voice(self), p == 0xff ? 0 : 0xff - p);
    }
}
