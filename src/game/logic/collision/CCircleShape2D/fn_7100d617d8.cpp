// game::logic::collision::CCircleShape2D unit (tools/tu_map.py): the circle's box in the world, its centre (the
// origin) moved by a transform's translation and grown by the radius (+0xa8), at the translation's z.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector3D;

struct UnkCircleBox3_ld617d8 {
    CVector3D min;
    CVector3D max;
};

struct UnkCircleTransform_ld617d8 {
    f32 m[3][4];
};

struct UnkCircleShape2D_ld617d8 {
    u8 pad_0x0[0xa8];
    f32 radius;  // 0xa8
};

// A 2D box from its centre and size.
struct UnkCircleBox2_ld617d8 {
    f32 min_x, min_y;
    f32 max_x, max_y;

    UnkCircleBox2_ld617d8(f32 cx, f32 cy, f32 w, f32 h) {
        f32 hw = w * 0.5f;
        f32 hh = h * 0.5f;
        min_x = cx - hw;
        min_y = cy - hh;
        max_x = cx + hw;
        max_y = cy + hh;
    }
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
UnkCircleBox3_ld617d8 fn_7100d617d8(const UnkCircleShape2D_ld617d8* self, const UnkCircleTransform_ld617d8* transform) {
    f32 size = self->radius + self->radius;
    UnkCircleBox2_ld617d8 box(transform->m[0][3] + 0.0f, transform->m[1][3] + 0.0f, size, size);
    f32 z = transform->m[2][3];
    return UnkCircleBox3_ld617d8{{box.min_x, box.min_y, z}, {box.max_x, box.max_y, z}};
}
