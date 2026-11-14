// From unit 48 of tools/tu_map.py: puts the vibration player's fade, gains, flags and both
// modulations back to their defaults without touching the SDK player (the tail of fn_7100070dd4,
// src/base/input/UnkRumblePlayer_Reset_b67a1.cpp). Called by fn_710006fbb0. Layout placeholders.
#include "base/math/UnkZeroVector3.h"
#include "types.h"


struct UnkRumbleVec_A1_b73 {
    f32 x, y, z;
};

struct UnkRumbleModulation_A1_b73 {
    f32 amplitude_low;
    f32 frequency_low;
    f32 amplitude_high;
    f32 frequency_high;
};

struct UnkRumbleDefaults_A1_b73 {
    u8 pad_0x0[0xd0];
    UnkRumbleModulation_A1_b73 modulation[2];  // 0xd0
    u8 pad_0xf0[0x70];
    f32 strength;  // 0x160
    f32 from;
    f32 to;        // 0x168
    f32 duration;
    f32 elapsed;   // 0x170
    UnkRumbleVec_A1_b73 offset;  // 0x174
    f32 balance;   // 0x180
    f32 gain;
    f32 current;   // 0x188
    f32 frequency;
    f32 field_0x190;
    u8 field_0x194[0xf];
    bool field_0x1a3;
    bool field_0x1a4;
    u8 field_0x1a5[4];
};
static_assert(__builtin_offsetof(UnkRumbleDefaults_A1_b73, field_0x1a5) == 0x1a5, "layout");

void fn_71000709bc(UnkRumbleDefaults_A1_b73* self) {
    self->to = 1.0f;
    self->strength = 1.0f;
    self->from = 1.0f;
    const u32* src = reinterpret_cast<const u32*>(&g_unk_7101647790);
    u32* dst = reinterpret_cast<u32*>(&self->offset);
    u32 x = src[0];
    dst[0] = x;
    u32 y = src[1];
    u32 z = src[2];
    dst[1] = y;
    dst[2] = z;
    self->balance = 1.0f;
    self->gain = 1.0f;
    self->current = 1.0f;
    self->frequency = 1.0f;
    self->field_0x190 = 1.0f;
    __builtin_memset(self->field_0x194, 0, sizeof(self->field_0x194));
    self->field_0x1a3 = true;
    self->field_0x1a4 = true;
    __builtin_memset(self->field_0x1a5, 0, sizeof(self->field_0x1a5));
    UnkRumbleModulation_A1_b73* m = self->modulation;
    m[0].amplitude_high = 1.0f;
    m[0].amplitude_low = 1.0f;
    m[0].frequency_low = 1.0f;
    m[0].frequency_high = 1.0f;
    m[1].amplitude_low = 1.0f;
    m[1].frequency_high = 1.0f;
    m[1].frequency_low = 1.0f;
    m[1].amplitude_high = 1.0f;
}
