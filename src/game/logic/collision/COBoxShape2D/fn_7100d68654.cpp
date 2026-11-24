// game::logic::collision::COBoxShape2D (tools/tu_map.py unit 2516): the shape's bounds in the world: its
// oriented box (+0xa8) placed by a transform as fn_7100d685c0 does, its 2D bounds (fn_7100110550), at the
// transform's depth for both corners. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/spatial/COBox2D.h"
#include "game/UnkOBoxPlacement.h"
#include "game/UnkOBoxShape2D.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

// An axis-aligned 2D box, returned in memory.
struct UnkAABox2D_ld68654 {
    base::math::CVector2D min;
    base::math::CVector2D max;
    UnkAABox2D_ld68654(const UnkAABox2D_ld68654& o) : min{o.min.x, o.min.y}, max{o.max.x, o.max.y} {}
};

// The axis-aligned bounds of an oriented box.
UnkAABox2D_ld68654 fn_7100110550(const base::spatial::COBox2D* self);

struct UnkWorldBounds_ld68654 {
    base::math::CVector3D min;
    base::math::CVector3D max;
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
UnkWorldBounds_ld68654 fn_7100d68654(const UnkOBoxShape2D* self, const UnkXform* xf) {
    UnkOBoxPlacement box = self->placement;
    base::math::CVector2D t = {xf->m[0][3], xf->m[1][3]};
    base::math::CVector2D center = {box.center.x + t.x, box.center.y + t.y};
    box.center = center;
    SetOBox2DAngleAndRebuildAxes(&box, fn_7100076acc(xf).z + box.angle);
    UnkAABox2D_ld68654 bounds = fn_7100110550(&box);
    f32 z = xf->m[2][3];
    UnkWorldBounds_ld68654 result;
    result.min = {bounds.min.x, bounds.min.y, z};
    result.max = {bounds.max.x, bounds.max.y, z};
    return result;
}
