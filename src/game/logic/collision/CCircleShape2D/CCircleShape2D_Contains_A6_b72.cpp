// game::logic::collision::CCircleShape2D unit (tools/tu_map.py): whether a 2D point lies strictly inside the
// circle whose centre is the projected translation of the 3x4 transform at +8 and whose radius is at +0xa8.
// Names are placeholders.
#include "base/math/CVector.h"
#include "types.h"

struct UnkCircleShape2D_A6_b72 {
    char pad_0x0[0x8];
    f32 transform[3][4];
    char pad_0x38[0xa8 - 0x38];
    f32 radius;
};

bool fn_7100d6195c(UnkCircleShape2D_A6_b72* self, const base::math::CVector2D* p) {
    base::math::CVector2D c;
    f32 tx = self->transform[0][3];
    f32 ty = self->transform[1][3];
    f32 tz = self->transform[2][3];
    base::math::CVector3D t = {tx, ty, tz};
    fn_710007b430(&c, &t);
    f32 dx = p->x - c.x;
    f32 dy = p->y - c.y;
    f32 d2 = dx * dx + dy * dy;
    return d2 < self->radius * self->radius;
}
