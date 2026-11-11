// behaviortree::CIsAtDistanceXCondition's unit (tools/tu_map.py): fetches two positions through
// fn_710039525c (keys at +0x38 and +0x50; kept at +0x40 and +0x58) and answers whether the second
// is within reach of the first: no when both coincide with the global position (within 0.001 on
// every axis), else the horizontal distance must be within +0x64 and, unless the vertical
// tolerance (+0x68) is zero, the second position's y must lie within that tolerance of the first's.
// Names and layouts are placeholders.
#include "base/math/UnkZeroVector3.h"
#include "types.h"

using UnkVec3f = base::math::CVector3D;

struct UnkSelf {
    u8 pad_0x0[0x38];
    u8 field_0x38[8];
    UnkVec3f field_0x40;
    u8 pad_0x4c[4];
    u8 field_0x50[8];
    UnkVec3f field_0x58;
    f32 field_0x64;
    f32 field_0x68;
};

void fn_710039525c(UnkSelf* self, void* arg, UnkVec3f* out);

#define UNK_NEAR(a, b) ((a) - 0.001f <= (b) && (a) + 0.001f >= (b))

bool fn_710039d65c(UnkSelf* self) {
    UnkVec3f a;
    a.x = 0;
    a.y = 0;
    a.z = 0;
    UnkVec3f b;
    b.x = 0;
    b.y = 0;
    b.z = 0;
    fn_710039525c(self, self->field_0x38, &a);
    fn_710039525c(self, self->field_0x50, &b);
    self->field_0x40.x = a.x;
    self->field_0x40.y = a.y;
    self->field_0x40.z = a.z;
    self->field_0x58.x = b.x;
    self->field_0x58.y = b.y;
    self->field_0x58.z = b.z;
    if (__builtin_expect(UNK_NEAR(a.x, g_unk_710164779c.x) && UNK_NEAR(a.y, g_unk_710164779c.y) &&
                             UNK_NEAR(a.z, g_unk_710164779c.z) && UNK_NEAR(b.x, g_unk_710164779c.x) &&
                             UNK_NEAR(b.y, g_unk_710164779c.y) && UNK_NEAR(b.z, g_unk_710164779c.z),
                         0))
        return false;
    f32 dx = __builtin_fabsf(a.x - b.x);
    f32 tolerance = self->field_0x68;
    bool below = b.y - tolerance < a.y;
    bool above = tolerance + b.y > a.y;
    bool in_band = tolerance == 0.0f ? true : (above & below);
    if (!(dx <= self->field_0x64))
        return false;
    return in_band;
}
