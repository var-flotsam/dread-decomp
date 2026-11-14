#include "base/input/UnkHidVibration.h"
#include "base/math/UnkZeroVector3.h"
#include "types.h"


struct UnkRumbleModulation_ovb0186 {
    f32 amplitude_low;
    f32 frequency_low;
    f32 amplitude_high;
    f32 frequency_high;
};

struct UnkRumbleLoad_ovb0186 : nn::hid::VibrationPlayer {
    u8 pad_0xf0[0x70];
    f32 strength;  // 0x160
    f32 from;
    f32 to;  // 0x168
    f32 duration;
    f32 elapsed;    // 0x170
    f32 offset[3];  // 0x174
    f32 balance;    // 0x180
    f32 gain;
    f32 current;  // 0x188
    f32 frequency;
    f32 field_0x190;
    u32 field_0x194;
    u32 field_0x198;
    u8 pad_0x19c[7];
    bool dirty;  // 0x1a3
    bool field_0x1a4;
    u8 field_0x1a5[3];
    bool field_0x1a8;
};
static_assert(__builtin_offsetof(UnkRumbleLoad_ovb0186, field_0x1a8) == 0x1a8, "layout");

struct UnkVibrationBlob_ovb0186 {
    u8 pad_0x0[8];
    const void* data;  // 8
    u32 size;          // 0x10
};

// A vibration description: offsets, gains and the file it plays.
struct UnkVibrationDesc_ovb0186 {
    u8 pad_0x0[0x48];
    f32 offset[3];  // 0x48
    u8 pad_0x54[0x68 - 0x54];
    UnkVibrationBlob_ovb0186* blob;  // 0x68
    u8 pad_0x70[4];
    f32 field_0x74;
    f32 field_0x78;
    f32 field_0x7c;
    f32 field_0x80;
    f32 field_0x84;
    u8 pad_0x88[8];
    f32 field_0x90;
    u8 pad_0x94[0xa1 - 0x94];
    bool loop;  // 0xa1
    bool field_0xa2;
};

static inline UnkRumbleModulation_ovb0186* Modulations(UnkRumbleLoad_ovb0186* self) {
    return reinterpret_cast<UnkRumbleModulation_ovb0186*>(reinterpret_cast<u8*>(self) + 0xd0);
}

// NON_MATCHING(minor): the same instructions in another order or with other registers.
void fn_71000708c8(UnkRumbleLoad_ovb0186* self, const UnkVibrationDesc_ovb0186* desc) {
    self->to = 1.0f;
    self->strength = 1.0f;
    self->from = 1.0f;
    const u32* src = reinterpret_cast<const u32*>(&g_unk_7101647790);
    u32* dst = reinterpret_cast<u32*>(self->offset);
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
    __builtin_memset(&self->field_0x194, 0, 15);
    self->dirty = true;
    self->field_0x1a4 = true;
    u32 one = 1;
    __builtin_memcpy(self->field_0x1a5, &one, 4);
    UnkRumbleModulation_ovb0186* m = Modulations(self);
    m[0].amplitude_high = 1.0f;
    m[0].amplitude_low = 1.0f;
    m[0].frequency_low = 1.0f;
    m[0].frequency_high = 1.0f;
    m[1].amplitude_low = 1.0f;
    m[1].frequency_high = 1.0f;
    m[1].frequency_low = 1.0f;
    m[1].amplitude_high = 1.0f;
    self->offset[0] = desc->offset[0];
    self->offset[1] = desc->offset[1];
    self->offset[2] = desc->offset[2];
    self->balance = desc->field_0x84;
    self->gain = desc->field_0x7c;
    self->frequency = desc->field_0x80;
    self->field_0x190 = desc->field_0x90;
    self->field_0x194 = *reinterpret_cast<const u32*>(&desc->field_0x78);
    self->field_0x198 = *reinterpret_cast<const u32*>(&desc->field_0x74);
    self->field_0x1a8 = desc->field_0xa2;
    self->Load(desc->blob->data, desc->blob->size);
    self->SetLoop(desc->loop);
}
