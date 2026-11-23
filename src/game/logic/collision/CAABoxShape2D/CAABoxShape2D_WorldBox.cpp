// game::logic::collision::CAABoxShape2D unit: the shape's 2D box (min at +0xa8, max at +0xb0) moved
// by a transform's translation (+0xc, +0x1c, +0x2c of a 3x4 matrix) into a 3D box at the
// translation's z. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkBox3.h"
#include "game/UnkBoxTransform.h"
#include "game/UnkBoxVec2.h"
#include "types.h"

using base::math::CVector3D;
struct UnkAABoxShape2D {
    u8 pad_0x0[0xa8];
    UnkBoxVec2 min;  // 0xa8
    UnkBoxVec2 max;  // 0xb0
};

UnkBox3 fn_7100d5e39c(const UnkAABoxShape2D* self, const UnkBoxTransform* transform) {
    UnkBoxVec2 min = self->min;
    UnkBoxVec2 max = self->max;
    f32 x = transform->m[0][3];
    f32 y = transform->m[1][3];
    f32 z = transform->m[2][3];
    return UnkBox3{{min.x + x, min.y + y, z}, {max.x + x, max.y + y, z}};
}
