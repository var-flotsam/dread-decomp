// game::logic::collision::COBoxShape2D: fn_7100d685c0 returns the shape's oriented box (+0xa8) placed by a
// transform: moved by its translation and turned by its z angle (fn_7100076acc) on top of the box's own
// angle (fn_7100110664). Names and layouts are placeholders.
#include "base/spatial/COBox2D.h"
#include "game/UnkXformNode.h"
#include "types.h"

// The box copied member by member (as UnkPlacement2D in src/game/UnkCollisionContacts.cpp).
struct UnkOBoxPlacement : base::spatial::COBox2D {
    UnkOBoxPlacement(const UnkOBoxPlacement& o)
        : COBox2D{{o.center.x, o.center.y}, {o.axis_x.x, o.axis_x.y}, {o.axis_y.x, o.axis_y.y},
                  {o.size.x, o.size.y}, o.offset, o.angle} {}
};

struct UnkOBoxShape2D {
    u8 pad_0x0[0xa8];
    UnkOBoxPlacement placement;  // 0xa8
};

UnkVec3f fn_7100076acc(const UnkXform* x);  // the rotation angles of a transform

UnkOBoxPlacement fn_7100d685c0(const UnkOBoxShape2D* self, const UnkXform* xf) {
    UnkOBoxPlacement box = self->placement;
    base::math::CVector2D t = {xf->m[0][3], xf->m[1][3]};
    base::math::CVector2D center = {t.x + box.center.x, t.y + box.center.y};
    box.center = center;
    fn_7100110664(&box, fn_7100076acc(xf).z + box.angle);
    return box;
}
