// base::snd::CSoundSystemATK's unit (unit 99): setters of a playing sound (src/base/snd/CSoundSystemATK.cpp
// calls it the sound with an enabled flag at +0x16c): its volume through the middleware handle
// (+0x28, never below 0), and two clamped values of its voice (+0x30): one in [0, 2] at +0xb4 and
// one in [-2, 2] at +0xbc. Names and layouts are placeholders.
#include "types.h"

void fn_710120e5d0(void* sound, f32 value, u32 x1);

struct UnkAtkVoice99 {
    u8 pad_0x0[0xb4];
    f32 field_0xb4;
    u8 pad_0xb8[0x4];
    f32 field_0xbc;
};

struct UnkAtkSound99 {
    u8 pad_0x0[0x28];
    void** handle;          // 0x28
    UnkAtkVoice99* voice;   // 0x30
    u8 pad_0x38[0x134];
    bool enabled;  // 0x16c
};
static_assert(__builtin_offsetof(UnkAtkSound99, enabled) == 0x16c, "UnkAtkSound99 layout");

void fn_71000f4c9c(UnkAtkSound99* self, f32 volume) {
    if (self->enabled && *self->handle)
        fn_710120e5d0(*self->handle, volume < 0.0f ? 0.0f : volume, 0);
}

void fn_71000f4cc4(UnkAtkSound99* self, f32 value) {
    if (self->enabled)
        self->voice->field_0xb4 = value <= 0.0f ? 0.0f : (value >= 2.0f ? 2.0f : value);
}

void fn_71000f4d98(UnkAtkSound99* self, f32 value) {
    if (self->enabled)
        self->voice->field_0xbc = value <= -2.0f ? -2.0f : (value >= 2.0f ? 2.0f : value);
}
