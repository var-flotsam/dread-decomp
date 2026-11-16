// CAIGotoPointNavigationState::STarget unit: after fn_7100703990 succeeds, checks whether two points lie in nearly
// opposite directions (2D dot of the normalised offsets below -0.98) as seen from the object's position
// at +0x38, and then writes that position to the output. Names and layouts are placeholders.

#include "base/math/CVectorOps.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkGotoTargetPos {
    u8 pad_0x0[0x38];
    CVector3D position;
};

bool fn_7100703990(UnkGotoTargetPos* self, const CVector3D* a, const CVector3D* b, CVector3D* out);

static void DirectionFrom(CVector2D& v, const CVector3D& position, const CVector3D& point) {
    CVector3D d = position - point;
    fn_710007b430(&v, &d);
    Normalize(v);
}

bool fn_71007026f0(UnkGotoTargetPos* self, const CVector3D* a, const CVector3D* b, CVector3D* out) {
    bool ok = fn_7100703990(self, a, b, out);
    if (ok) {
        CVector2D da;
        DirectionFrom(da, self->position, *a);
        CVector2D db;
        DirectionFrom(db, self->position, *b);
        if (Dot(da, db) < -0.98f) {
            out->x = self->position.x;
            out->y = self->position.y;
            out->z = self->position.z;
        }
    }
    return ok;
}
