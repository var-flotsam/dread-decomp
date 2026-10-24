// Per-frame step of a component: when the owner (+0x18) has a target at +0x298 and a record at +0x218,
// takes the vector from the owner's position to the record's position, picks the owner's matrix
// column (words at +8, +0x18, +0x28; negated when it points against that vector) and hands it in a
// parameter block (kind 5) to fn_71006f4b0c with 0.
#include "game/UnkActor.h"
#include "game_s2/UnkParams.h"
#include "types.h"

struct UnkComponent {
    u8 pad_0x0[0x18];
    UnkActor* field_0x18;
};

struct UnkColumn {
    u8 pad_0x0[8];
    u32 field_0x8;
    u8 pad_0xc[0xc];
    u32 field_0x18;
    u8 pad_0x1c[0xc];
    u32 field_0x28;
};

// The kind-5 block (game_s2/UnkParams.h) with its first fields seen as words.
struct UnkParams_71005136cc {
    void** vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u8 field_0x14;
    u8 pad_0x15[3];
    u32 pad_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u8 field_0x20[16];
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    u8 pad_0x38;
    u8 field_0x39;
    u8 pad_0x3a;
    u8 field_0x3b;
    u8 pad_0x3c;
    u8 field_0x3d;
};

UnkColumn* fn_710002614c(void* object);

static inline f32 AsF32(u32 bits) {
    f32 value;
    __builtin_memcpy(&value, &bits, 4);
    return value;
}

void fn_71005136cc(UnkComponent* self) {
    UnkActor* owner = self->field_0x18;
    if (owner->field_0x298 && owner->field_0x218) {
        UnkActor* position = fn_7100d7c990(owner);
        UnkActor* o = self->field_0x18;
        f32 px = position->field_0x44;
        f32 py = position->field_0x54;
        f32 pz = position->field_0x64;
        f32 ox = o->field_0x44;
        f32 oy = o->field_0x54;
        f32 oz = o->field_0x64;
        f32 dx = px - ox;
        f32 dy = py - oy;
        f32 dz = pz - oz;
        UnkColumn* column = fn_710002614c(o->field_0x38);
        u32 x = column->field_0x8;
        u32 y = column->field_0x18;
        u32 z = column->field_0x28;
        if (!(dx * AsF32(x) + dy * AsF32(y) + dz * AsF32(z) > 0.0f)) {
            x ^= 0x80000000;
            y ^= 0x80000000;
            z ^= 0x80000000;
        }
        UnkParams_71005136cc params;
        params.field_0x30 = 0;
        params.field_0x1c = 0;
        __builtin_memset(params.field_0x20, 0, 16);
        params.vtable = g_unk_7101a25b10 + 2;
        params.field_0x39 = 0;
        params.field_0x3b = 0;
        params.field_0x34 = 5;
        params.field_0x8 = x;
        params.field_0x10 = z;
        params.field_0x3d = 0;
        params.field_0xc = y;
        params.field_0x14 = 1;
        fn_71006f4b0c(self->field_0x18->field_0x298, &params, 0);
    }
}
