// Unit 99 (the sound system's): queue a voice's start command with a snapshot of its parameters,
// command 0x12 for a voice with the flag at +0x120 set and command 0x6 (positional) for the rest.
#include "base/snd/UnkSoundSystemATK.h"
#include "base/tunable/CTunable.h"

using base::snd::UnkSoundSystemATK;

struct UnkSoundCue_b56 {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkSoundVoice_b56 {
    u8 pad_0x0[0x8];
    f32 volume;  // +0x8
    u8 pad_0xc[0x38];
    u32 field_0x44;
    u8 pad_0x48[0x48];
    UnkSoundCue_b56* cue;  // +0x90
    f32 pos[3];            // +0x98
    u8 pad_0xa4[0xc];
    u32 field_0xb0;
    u32 field_0xb4;
    u8 pad_0xb8[0xc];
    u32 id;  // +0xc4
    u8 pad_0xc8[0x4];
    u32 field_0xcc;
    u32 field_0xd0;
    u32 field_0xd4;
    u32 field_0xd8;
    u32 field_0xdc;
    u8 pad_0xe0[0x4];
    f32 field_0xe4;
    f32 field_0xe8;
    u8 pad_0xec[0xc];
    u32 field_0xf8;
    u8 pad_0xfc[0x4];
    u32 field_0x100;
    u32 field_0x104;
    u8 pad_0x108[0x14];
    u8 field_0x11c;
    u8 pad_0x11d[0x2];
    u8 field_0x11f;
    bool field_0x120;
};

f32 fn_7100105074(UnkSoundVoice_b56* voice, const f32* listener);

struct UnkVoiceStartPayload_b56 {
    u32 id;
    u64 cue;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    f32 volume;
    f32 time;
    u32 field_0x34;
};

struct UnkVoiceStart3DPayload_b56 {
    u32 id;
    u64 cue;
    u32 field_0x10;
    u32 field_0x14;
    f32 pos[3];
    f32 volume;
    f32 field_0x28;
    f32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    f32 time;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;
    u32 field_0x48;
};

void fn_71000ff768(UnkSoundSystemATK* self, UnkSoundVoice_b56* voice, f32 time) {
    fn_7100132cd0();
    if (!voice)
        return;
    if (voice->field_0x120) {
        UnkVoiceStartPayload_b56 payload;
        payload.id = voice->id;
        payload.cue = voice->cue->field_0x30;
        payload.field_0x10 = voice->field_0xb0;
        payload.field_0x14 = voice->field_0x11c;
        payload.field_0x18 = voice->field_0xd4;
        payload.field_0x1c = voice->field_0xcc;
        payload.field_0x20 = voice->field_0xd0;
        payload.field_0x24 = voice->field_0xd8;
        payload.field_0x28 = voice->field_0xdc;
        payload.volume = voice->volume;
        payload.time = time;
        payload.field_0x34 = voice->field_0x104;
        fn_71000faac4(self, 0x12, &payload);
    } else {
        UnkVoiceStart3DPayload_b56 payload;
        payload.id = voice->id;
        payload.cue = voice->cue->field_0x30;
        payload.field_0x10 = voice->field_0xb0;
        payload.field_0x14 = voice->field_0x11f;
        payload.pos[0] = voice->pos[0];
        payload.pos[1] = voice->pos[1];
        payload.pos[2] = voice->pos[2];
        payload.volume = voice->volume * fn_7100105074(voice, reinterpret_cast<const f32*>(reinterpret_cast<char*>(self) + 0xac));
        payload.field_0x28 = voice->field_0xe8;
        payload.field_0x2c = voice->field_0xe4;
        payload.field_0x30 = voice->field_0xf8;
        payload.field_0x34 = voice->field_0x44;
        payload.time = time;
        payload.field_0x3c = voice->field_0x100;
        payload.field_0x40 = voice->field_0x104;
        payload.field_0x44 = voice->field_0xb4;
        payload.field_0x48 = voice->field_0x11c;
        fn_71000faac4(self, 0x6, &payload);
    }
}
