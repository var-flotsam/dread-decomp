// CFlockingSwarmControllerComponent unit: keeps a point on its side of a link between two swarm
// members: when the link's gap is negative, the point is pushed along the link's normal, turned
// towards the point's side, by the gap (at most field_0x5c). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkSwarmMember_l742290 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const CVector3D* vf_0x18();  // position
};

struct UnkSwarmLink_l742290 {
    u8 pad_0x0[0x40];
    f32 field_0x40;
    f32 field_0x44;
    u64 field_0x48;  // first member's id
    u64 field_0x50;  // second member's id
    u8 pad_0x58[4];
    f32 field_0x5c;
    u8 pad_0x60[0x94 - 0x60];
    UnkVec3f field_0x94;  // normal

    UnkVec3f Normal() const { return field_0x94; }
};
static_assert(__builtin_offsetof(UnkSwarmLink_l742290, field_0x94) == 0x94, "layout");

struct UnkFlockingSwarm_l742290 {
    u8 pad_0x0[0x390];
    u32 field_0x390;
};

UnkSwarmMember_l742290* fn_7100749e44(UnkFlockingSwarm_l742290* self, const u64* id);

// The projection of a vector taken by value.
static inline void XY_l742290(CVector2D* xy, UnkVec3f v) {
    fn_710007b430(xy, reinterpret_cast<const CVector3D*>(&v));
}

void fn_7100742290(UnkFlockingSwarm_l742290* self, UnkSwarmLink_l742290* link, CVector3D* pos) {
    if (self->field_0x390 == 1)
        return;
    u64 firstId = link->field_0x48;
    u64 secondId = link->field_0x50;
    UnkSwarmMember_l742290* first = fn_7100749e44(self, &firstId);
    UnkSwarmMember_l742290* second = fn_7100749e44(self, &secondId);
    if (!first || !second)
        return;

    CVector2D start;
    fn_710007b430(&start, first->vf_0x18());
    CVector2D end;
    fn_710007b430(&end, second->vf_0x18());
    UnkSegment2D segment(start, end);

    CVector2D normal;
    f32 dx, dy;
    {
        CVector2D point;
        fn_710007b430(&point, pos);
        dx = point.x - start.x;
        dy = point.y - start.y;
    }

    XY_l742290(&normal, link->Normal());
    CVector2D dir;
    XY_l742290(&dir, link->Normal());
    f32 dot = dx * dir.x + dy * dir.y;
    f32 side = (dot > 0.0f) - (dot < 0.0f);
    normal.x *= side;
    normal.y *= side;

    f32 gap = link->field_0x44 - (link->field_0x40 + link->field_0x5c);
    if (!(gap < 0.0f))
        return;
    gap = __builtin_fabsf(gap);
    f32 push = link->field_0x5c < gap ? link->field_0x5c : gap;

    // The flat normal, negated and scaled.
    CVector3D offset = {-normal.x, -normal.y, -0.0f};
    pos->x += push * offset.x;
    pos->y += push * offset.y;
    pos->z += push * offset.z;
}
