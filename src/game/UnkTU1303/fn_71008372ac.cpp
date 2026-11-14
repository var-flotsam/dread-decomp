// Unit 1303: one entry of a spread of shots: when the table (+0x28, rows of +0x24 entries, offset by
// +0x3c) has the entry set, gives its offset (a direction scaled by a strength, rolled between the
// minimum and 1 when that is below 1) with the spread across the entries, and the scale and extra
// values to go with it. Names and layouts are placeholders.
#include "types.h"

extern "C" int rand();

struct UnkVec3_ovb0262g {
    f32 x, y, z;
};

struct UnkTunableValue_ovb0262g {
    u8 pad_0x0[0x2c];
    f32 value;  // 0x2c
};
extern UnkTunableValue_ovb0262g g_unk_71726af060;

struct UnkSpreadEntry_ovb0262g {
    bool ok;  // 0x0
    UnkVec3_ovb0262g offset;  // 0x4
    f32 scale;  // 0x10
    f32 extra;  // 0x14
    UnkSpreadEntry_ovb0262g() : ok(false), offset{0.0f, 0.0f, 0.0f}, scale(0.0f), extra(0.0f) {}
};

struct UnkSpread_ovb0262g {
    f32 strength;  // 0x0
    f32 strength_min;  // 0x4
    u8 pad_0x8[0x8];
    f32 scale_min;  // 0x10
    f32 scale_max;  // 0x14
    f32 scale_random;  // 0x18
    f32 extra;  // 0x1c
    u32 count;  // 0x20
    u32 row_size;  // 0x24
    u8* table;  // 0x28
    u8 pad_0x30[8];
    u32 disabled;  // 0x38
    u32 row_offset;  // 0x3c
};

static inline f32 RandUnit() {
    return (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
}

static inline f32 RandRange(f32 lo, f32 hi) {
    f32 r = RandUnit();
    return lo + (hi - lo) * r;
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
UnkSpreadEntry_ovb0262g fn_71008372ac(UnkSpread_ovb0262g* self, u32 index, const UnkVec3_ovb0262g* direction,
                                      const UnkVec3_ovb0262g* spread) {
    f32 range = g_unk_71726af060.value;
    UnkSpreadEntry_ovb0262g result;
    if (self->disabled) {
        result.ok = false;
        return result;
    }
    result.ok = self->table[self->row_size * index + self->row_offset] != 0;
    if (!result.ok)
        return result;
    f32 strength = self->strength;
    if (self->strength_min < 1.0f)
        strength *= RandRange(self->strength_min, 1.0f);
    f32 x = strength * direction->x;
    f32 y = strength * direction->y;
    f32 z = strength * direction->z;
    result.offset.x = x;
    result.offset.y = y;
    result.offset.z = z;
    if (self->count < 2)
        return result;
    if (range > -range) {
        f32 step = (range + range) / (f32)(self->count - 1);
        f32 position = step * (f32)index - range;
        f32 sx = spread->x, sy = spread->y, sz = spread->z;
        f32 px = position * sx;
        f32 py = position * sy;
        f32 pz = position * sz;
        result.offset.x = px + result.offset.x;
        result.offset.y = py + result.offset.y;
        result.offset.z = pz + result.offset.z;
        if (self->scale_min <= self->scale_max) {
            f32 t = (range + position) / (range + range);
            f32 u = !(t <= 0.0f) ? (!(t >= 1.0f) ? t : 1.0f) : 0.0f;
            f32 scale = self->scale_min + u * (self->scale_max - self->scale_min);
            if (self->scale_random < 1.0f)
                scale += RandRange(0.5f, 1.0f);
            result.scale = scale;
        }
    }
    result.extra = self->extra;
    return result;
}
