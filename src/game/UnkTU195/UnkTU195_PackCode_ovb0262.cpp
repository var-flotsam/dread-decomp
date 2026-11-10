// Unit 195: packs a 32-bit code for the owner's kind (+0x8): kind 1 the distance between two points
// in hundredths, kind 2 a distance-based fade level (falling with the distance) with the value, the
// selector from the table and a clamped count, kind 3 bits taken from the context. The selector comes
// from a table indexed by half the (even) argument minus one. Names and layouts are placeholders.
#include "types.h"

extern "C" float sqrtf(float);
extern const u32 g_unk_7101648094[] __attribute__((visibility("hidden")));

struct UnkPackKind_ovb0262k {
    u8 pad_0x0[8];
    u32 kind;  // 0x8
};

struct UnkPackSource_ovb0262k {
    u8 pad_0x0[0x40];
    u32 bits;  // 0x40
};

struct UnkPackContext_ovb0262k {
    u8 pad_0x0[0x1c];
    u32 count;  // 0x1c
    u8 pad_0x20[0x370];
    UnkPackSource_ovb0262k* source;  // 0x390
    u8 pad_0x398[0xc];
    u16 field_0x3a4;
};
static_assert(__builtin_offsetof(UnkPackContext_ovb0262k, field_0x3a4) == 0x3a4, "layout");

struct UnkVec3_ovb0262k {
    f32 x, y, z;
};

static inline f32 Distance(const UnkVec3_ovb0262k* a, const UnkVec3_ovb0262k* b) {
    f32 dx = a->x - b->x;
    f32 dy = a->y - b->y;
    f32 dz = a->z - b->z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

u32 fn_7100168f30(UnkPackKind_ovb0262k* self, u32 argument, u32 value, UnkPackContext_ovb0262k* context,
                  void* unused, const UnkVec3_ovb0262k* a, const UnkVec3_ovb0262k* b) {
    u32 index = argument - 2;
    index = (index >> 1) | (index << 31);
    u32 selector = index <= 6 ? g_unk_7101648094[(s32)index] : 0;
    switch (self->kind) {
    case 3: {
        u32 packed = context->source->bits & 0x3fff;
        packed = (packed & 0x3fffffff) | ((selector & 3) << 30);
        packed = (packed & 0xc0003fff) | ((u32)context->field_0x3a4 << 14);
        return packed;
    }
    case 2: {
        f32 v = Distance(a, b) / 100.0f + 1000.0f;
        v = !(v < 0.0f) ? __builtin_floorf(v * 0.1f) * 10.0f : 0.0f;
        f32 t = v / 999999.0f;
        t = !(t >= 1.0f) ? t : 1.0f;
        u32 level = (u32)((1.0f - t) * 65535.0f);
        u32 clamped = context->count;
        if (clamped >= 0x3f)
            clamped = 0x3f;
        u32 packed = (value & 0xff) << 16;
        packed = (packed & 0x00ffffff) | ((selector & 0xff) << 24);
        return (packed | (clamped << 26)) | level;
    }
    case 1:
        return (u32)(Distance(a, b) * 0.01f);
    default:
        return 0;
    }
}
