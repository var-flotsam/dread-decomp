// Unit 2563 (CMinimapData hint): recomputes the 2D bounding box of the projected 3D points.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkBoundsOwner_e3e1c0 {
    CVector3D* points;  // 0x0
    u32 count;          // 0x8
    u8 pad_0xc[0x44];
    CVector2D box_min;  // 0x50
    CVector2D box_max;  // 0x58
};

void fn_7100e3e1c0(UnkBoundsOwner_e3e1c0* self) {
    self->box_min.x = 3.4028235e+38f;
    self->box_min.y = 3.4028235e+38f;
    self->box_max.x = -3.4028235e+38f;
    self->box_max.y = -3.4028235e+38f;
    CVector2D& lo = self->box_min;
    CVector2D& hi = self->box_max;
    for (CVector3D *v = self->points, *end = v + self->count; v != end; ++v) {
        CVector2D p;
        fn_710007b430(&p, v);
        f32 lx = lo.x < p.x ? lo.x : p.x;
        f32 ly = lo.y < p.y ? lo.y : p.y;
        lo.x = lx;
        lo.y = ly;
        f32 hx = hi.x > p.x ? hi.x : p.x;
        f32 hy = hi.y > p.y ? hi.y : p.y;
        hi.x = hx;
        hi.y = hy;
    }
}
