// Unit 308: initialises a record from a scale value and a 3-float vector, storing fixed-point copies.
// Names and layouts are placeholders.
#include "types.h"

extern char g_unk_7101a20220[];

struct UnkScaledRecord {
    void* vtable;
    f32 scale;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 vec[3];
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    u32 pad_0x34;
    s64 scaled;
    s32 fixed[3];
    u8 isZero;
    u8 field_0x4d;
};

// NON_MATCHING(minor): same instructions; scheduling and register choice differ (GOT load and int-to-float moves)
void fn_710021688c(f32 scale, UnkScaledRecord* self, const u32* vec) {
    char* vt = g_unk_7101a20220;
    self->scale = scale;
    self->field_0xc = 0;
    self->field_0x10 = 0;
    self->field_0x14 = 0;
    self->vtable = vt + 0x10;
    f32 x, y, z;
    u32 xb = vec[0];
    self->vec[0] = xb;
    __builtin_memcpy(&x, &xb, 4);
    u32 yb = vec[1];
    self->vec[1] = yb;
    u32 zb = vec[2];
    __builtin_memcpy(&y, &yb, 4);
    __builtin_memcpy(&z, &zb, 4);
    self->vec[2] = zb;
    if (scale >= 0.0f) {
        self->field_0x24 = 0.0f;
        self->field_0x28 = scale;
        self->field_0x2c = 0.0f;
        self->field_0x30 = scale;
    }
    s64 s = (s64)(scale * 10000.0f);
    self->scaled = s;
    self->isZero = s == 0;
    self->fixed[0] = (s32)(x * 10.0f);
    self->fixed[1] = (s32)(y * 10.0f);
    self->fixed[2] = (s32)(z * 10.0f);
    self->field_0x4d = 0;
}
