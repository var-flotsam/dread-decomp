// Asks fn_710039525c for a position (and keeps it at +0x40). When it is the same as the global
// position (within 0.001 on every axis) the answer is no; otherwise yes if the position lies in
// front of the object at +0x18, i.e. the vector to it from there has a positive component along
// the object's facing (the third column of its matrix).
#include "base/UnkMatrix44.h"
#include "types.h"
#include "base/math/UnkZeroVector3.h"

using UnkVec3f = base::math::CVector3D;

struct UnkFacing {
    u8 pad_0x0[0x38];
    u8 field_0x38[0xc];
    f32 x_0x44;
    u8 pad_0x48[0xc];
    f32 y_0x54;
    u8 pad_0x58[0xc];
    f32 z_0x64;
};

struct UnkSelf {
    u8 pad_0x0[0x18];
    UnkFacing* field_0x18;
    u8 pad_0x20[0x18];
    u8 field_0x38[8];
    UnkVec3f field_0x40;
};

void fn_710039525c(UnkSelf* self, void* arg, UnkVec3f* out);

#define UNK_NEAR(a, b) ((a) - 0.001f <= (b) && (a) + 0.001f >= (b))

bool fn_71003a1014(UnkSelf* self) {
    UnkVec3f p;
    p.x = 0;
    p.y = 0;
    p.z = 0;
    fn_710039525c(self, self->field_0x38, &p);
    self->field_0x40.x = p.x;
    self->field_0x40.y = p.y;
    self->field_0x40.z = p.z;
    if (UNK_NEAR(p.x, g_unk_710164779c.x) && UNK_NEAR(p.y, g_unk_710164779c.y) &&
        UNK_NEAR(p.z, g_unk_710164779c.z))
        return false;
    UnkFacing* facing = self->field_0x18;
    f32 fx = facing->x_0x44;
    f32 fy = facing->y_0x54;
    f32 fz = facing->z_0x64;
    f32 dx = p.x - fx;
    f32 dy = p.y - fy;
    f32 dz = p.z - fz;
    const UnkMatrix44& matrix = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(facing->field_0x38));
    f32 m0 = matrix.m[2];
    f32 m1 = matrix.m[6];
    f32 m2 = matrix.m[10];
    return dx * m0 + dy * m1 + dz * m2 > 0;
}
