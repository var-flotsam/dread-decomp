// Unit 2517 of tools/tu_map.py (a 2D collision shape): the shape's two corners (+0xa8, +0xb0) moved by a
// transform's translation (+0xc, +0x1c, +0x2c of a 3x4 matrix), ordered into the min and max of a 3D box at
// the translation's z. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector3D;

struct UnkBoxVec2 {
    f32 x;
    f32 y;
};
struct UnkBox3 {
    CVector3D min;
    CVector3D max;
};
struct UnkBoxTransform {
    f32 m[3][4];
};

struct UnkSegmentShape2D_ld694b0 {
    u8 pad_0x0[0xa8];
    UnkBoxVec2 a;  // 0xa8
    UnkBoxVec2 b;  // 0xb0
};

UnkBox3 fn_7100d694b0(const UnkSegmentShape2D_ld694b0* self, const UnkBoxTransform* transform) {
    UnkBoxVec2 a = self->a;
    UnkBoxVec2 b = self->b;
    f32 tx = transform->m[0][3];
    f32 ty = transform->m[1][3];
    f32 z = transform->m[2][3];
    f32 ax = a.x + tx;
    f32 ay = a.y + ty;
    f32 bx = b.x + tx;
    f32 by = b.y + ty;
    UnkBox3 box;
    box.min.z = z;
    box.max.z = z;
    box.min.x = bx > ax ? ax : bx;
    box.min.y = by > ay ? ay : by;
    box.max.x = bx < ax ? ax : bx;
    box.max.y = by < ay ? ay : by;
    return box;
}
