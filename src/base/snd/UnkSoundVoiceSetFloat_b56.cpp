// Unit 99 (the sound system's): one more (voice id, float) setter, the twin of fn_71000ffbb0
// (src/base/snd/UnkSoundVoiceSetFloat_b55.cpp) with commands 0x18/0xd.
#include "base/snd/UnkSoundSystemATK.h"
#include "base/tunable/CTunable.h"

using base::snd::UnkSoundSystemATK;

struct UnkSoundVoiceRef_b56 {
    u8 pad_0x0[0xc4];
    u32 id;  // +0xc4
    u8 pad_0xc8[0x58];
    bool field_0x120;
};

struct UnkVoiceFloatPayload_b56 {
    u32 id;
    f32 value;
};

void fn_71000ffb3c(UnkSoundSystemATK* self, UnkSoundVoiceRef_b56* voice, f32 value) {
    fn_7100132cd0();
    if (!voice)
        return;
    if (voice->field_0x120) {
        UnkVoiceFloatPayload_b56 payload = {voice->id, value};
        fn_71000faac4(self, 0x18, &payload);
    } else {
        UnkVoiceFloatPayload_b56 payload = {voice->id, value};
        fn_71000faac4(self, 0xd, &payload);
    }
}

struct UnkVoiceWordPayload_b56 {
    u32 id;
    u32 value;
};

void fn_71000ffd4c(UnkSoundSystemATK* self, UnkSoundVoiceRef_b56* voice, u32 value) {
    fn_7100132cd0();
    if (!voice)
        return;
    if (voice->field_0x120) {
        UnkVoiceWordPayload_b56 payload = {voice->id, value};
        fn_71000faac4(self, 0x1c, &payload);
    } else {
        UnkVoiceWordPayload_b56 payload = {voice->id, value};
        fn_71000faac4(self, 0x11, &payload);
    }
}

static inline f32 ClampMinFade_b56(f32 value) {
    return value < 0.06f ? 0.06f : value;
}

// Fade time clamped to at least 0.06: commands 0x14/0x7.
void fn_71000ff8c4(UnkSoundSystemATK* self, UnkSoundVoiceRef_b56* voice, f32 time) {
    fn_7100132cd0();
    if (!voice)
        return;
    if (voice->field_0x120) {
        UnkVoiceFloatPayload_b56 payload = {voice->id, ClampMinFade_b56(time)};
        fn_71000faac4(self, 0x14, &payload);
    } else {
        UnkVoiceFloatPayload_b56 payload = {voice->id, ClampMinFade_b56(time)};
        fn_71000faac4(self, 0x7, &payload);
    }
}

struct UnkVoiceFlagFadePayload_b56 {
    u32 id;
    u32 flag;
    f32 time;
};

// (flag, fade time): commands 0x16/0x9.
void fn_71000ff9c0(UnkSoundSystemATK* self, UnkSoundVoiceRef_b56* voice, bool flag, f32 time) {
    fn_7100132cd0();
    if (!voice)
        return;
    if (voice->field_0x120) {
        UnkVoiceFlagFadePayload_b56 payload = {voice->id, flag, ClampMinFade_b56(time)};
        fn_71000faac4(self, 0x16, &payload);
    } else {
        UnkVoiceFlagFadePayload_b56 payload = {voice->id, flag, ClampMinFade_b56(time)};
        fn_71000faac4(self, 0x9, &payload);
    }
}

struct UnkVoiceFlagValueFadePayload_b56 {
    u32 id;
    u32 flag;
    f32 value;
    f32 time;
};

// (flag, value, fade time): commands 0x1b/0x10.
void fn_71000ffc98(UnkSoundSystemATK* self, UnkSoundVoiceRef_b56* voice, bool flag, f32 value, f32 time) {
    fn_7100132cd0();
    if (!voice)
        return;
    if (voice->field_0x120) {
        UnkVoiceFlagValueFadePayload_b56 payload = {voice->id, flag, value, ClampMinFade_b56(time)};
        fn_71000faac4(self, 0x1b, &payload);
    } else {
        UnkVoiceFlagValueFadePayload_b56 payload = {voice->id, flag, value, ClampMinFade_b56(time)};
        fn_71000faac4(self, 0x10, &payload);
    }
}
