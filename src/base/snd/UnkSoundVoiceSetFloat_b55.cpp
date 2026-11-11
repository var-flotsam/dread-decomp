// Unit 99 (the sound system's): setters that queue a (voice id, value) command for a voice, one
// command for a voice with the flag at +0x120 set and another for the rest.
#include "base/snd/UnkSoundSystemATK.h"
#include "base/tunable/CTunable.h"

using base::snd::UnkSoundSystemATK;

struct UnkSoundVoiceRef {
    u8 pad_0x0[0xc4];
    u32 id;  // +0xc4
    u8 pad_0xc8[0x58];
    bool field_0x120;
};

struct UnkVoiceFloatPayload {
    u32 id;
    f32 value;
};

template <s32 kFlagged, s32 kPlain>
static __attribute__((always_inline)) void QueueVoiceFloat(UnkSoundSystemATK* self, UnkSoundVoiceRef* voice, f32 value) {
    fn_7100132cd0();
    if (!voice)
        return;
    if (voice->field_0x120) {
        UnkVoiceFloatPayload payload = {voice->id, value};
        fn_71000faac4(self, kFlagged, &payload);
    } else {
        UnkVoiceFloatPayload payload = {voice->id, value};
        fn_71000faac4(self, kPlain, &payload);
    }
}

void fn_71000ffbb0(UnkSoundSystemATK* self, UnkSoundVoiceRef* voice, f32 value) {
    QueueVoiceFloat<0x19, 0xe>(self, voice, value);
}

void fn_71000ffc24(UnkSoundSystemATK* self, UnkSoundVoiceRef* voice, f32 value) {
    QueueVoiceFloat<0x1a, 0xf>(self, voice, value);
}
