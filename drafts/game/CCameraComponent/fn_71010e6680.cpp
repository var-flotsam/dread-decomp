// Unit 2691 (camera component): casts from a point to the left and right along the camera area's collision
// shape and reports the width between the two hits and the room ahead and behind in the facing direction.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/CCollider.h"
#include "game/logic/collision/UnkBvhTree.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkWorld;
struct UnkQueryBox;
struct UnkMtx34;
UnkFlags fn_7100d56604(UnkWorld* self, const UnkQueryBox* box, UnkShape* shape, const UnkMtx34* xf, void* arg, u32 arg2);

extern void* g_unk_7101a22c60[];

static inline UnkShapeTester_A5_b68* Tester() {
    UnkShapeTester_A5_b68* t = g_unk_71726a47a0;
    if (!t) {
        t = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
        fn_7100d5134c(t);
        g_unk_71726a47a0 = t;
    }
    return t;
}

// A ray from a point to a far target, with its direction.
struct UnkRay_l0e6680 {
    CVector2D from;
    CVector2D to;
    CVector2D dir;
    UnkRay_l0e6680(const CVector2D& start, f32 targetX)
        : from{start.x, start.y}, to{targetX, 0.0f}, dir{to.x - from.x, to.y - from.y} {}
};

// What the ray hit (vtable g_unk_7101a22c60).
struct UnkRayHit_l0e6680 {
    void** vtable;
    s32 field_0x8;
    CVector3D point;
    CVector3D normal;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    f32 field_0x34;
    UnkRayHit_l0e6680()
        : vtable(g_unk_7101a22c60 + 2), field_0x8(-1), point(), normal(), field_0x24(0), field_0x28(0),
          field_0x2c(0), field_0x30(0), field_0x34(0) {}
};

struct UnkCameraCollider_l0e6680 {
    u8 pad_0x0[0x158];
    UnkShape* shape;  // 0x158
};

struct UnkCameraArea_l0e6680 {
    u8 pad_0x0[0x10];
    UnkCameraCollider_l0e6680* collider;  // 0x10
};

struct UnkCamera_l0e6680 {
    u8 pad_0x0[0xc38];
    UnkCameraArea_l0e6680* area;  // 0xc38
};

static inline UnkFlags Cast(UnkShapeTester_A5_b68* tester, const UnkRay_l0e6680& ray, UnkShape* shape,
                            const UnkMatrix44& xf, UnkRayHit_l0e6680* hit) {
    return fn_7100d56604(reinterpret_cast<UnkWorld*>(tester), reinterpret_cast<const UnkQueryBox*>(&ray), shape,
                         reinterpret_cast<const UnkMtx34*>(&xf), hit, 2);
}

bool fn_71010e6680(UnkCamera_l0e6680* self, const CVector3D* position, const f32* facing, f32* ahead, f32* behind,
                   f32* width) {
    UnkCameraArea_l0e6680* area = self->area;
    if (!area)
        return false;
    UnkCameraCollider_l0e6680* collider = area->collider;
    if (!collider)
        return false;

    CVector2D pos;
    fn_710007b430(&pos, position);
    UnkShape* shape = collider->shape;
    UnkShapeTester_A5_b68* tester = Tester();
    UnkMatrix44 xf = fn_7100d36a2c(reinterpret_cast<UnkShape*>(collider));

    UnkRay_l0e6680 left(pos, -100000.0f);
    UnkRayHit_l0e6680 leftHit;
    UnkFlags leftResult = Cast(tester, left, shape, xf, &leftHit);
    if (!(leftResult.bits & 1))
        return false;
    UnkRay_l0e6680 right(pos, 100000.0f);
    UnkRayHit_l0e6680 rightHit;
    UnkFlags rightResult = Cast(tester, right, shape, xf, &rightHit);
    if (!(rightResult.bits & 1))
        return false;

    CVector2D leftEdge;
    fn_710007b430(&leftEdge, &leftHit.point);
    CVector2D rightEdge;
    fn_710007b430(&rightEdge, &rightHit.point);
    *width = rightEdge.x - leftEdge.x;
    f32 toRight = rightEdge.x - pos.x;
    f32 toLeft = pos.x - leftEdge.x;
    *ahead = *facing > 0.0f ? toRight : toLeft;
    *behind = *facing > 0.0f ? toLeft : toRight;
    return true;
}
