// CCameraBoundaryCtrl unit: probes the boundary box's four sides with eight rays (four vertical, four
// horizontal) against the owner's collision shape and writes into out how far the box must move to clear
// what the rays hit, by which corner or sides were hit. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU365.h"
#include "game/UnkTU55.h"
#include "game/logic/collision/CCollider.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

extern const base::math::CVector2D g_unk_7101647780;
extern char g_unk_7101a22c60[];

struct UnkShape;
struct UnkVec3;
struct UnkWorld;
struct UnkQueryBox;
struct UnkMtx34;

struct UnkFlags {
    u32 bits;
    UnkFlags() : bits(0) {}
    UnkFlags(const UnkFlags& o) : bits(o.bits) {}
    UnkFlags& operator=(const UnkFlags& o) {
        bits = o.bits;
        return *this;
    }
};
UnkFlags fn_7100d56604(UnkWorld* self, const UnkQueryBox* box, UnkShape* shape, const UnkMtx34* xf, void* arg, u32 arg2);

struct UnkBoundaryShape_l7659f8 {
    u8 pad_0x0[0x158];
    UnkShape* collection;
};

struct UnkBoundaryCollider_l7659f8 {
    u8 pad_0x0[0x10];
    UnkBoundaryShape_l7659f8* shape;
};

struct UnkBoundaryEntity_l7659f8 {
    u8 pad_0x0[0xc38];
    UnkBoundaryCollider_l7659f8* collider;
};

struct UnkBoundaryOwner_l7659f8 {
    u8 pad_0x0[0x10];
    UnkBoundaryEntity_l7659f8* entity;
};

struct UnkCameraBoundary_l7659f8 {
    u8 pad_0x0[0xb0];
    UnkBoundaryOwner_l7659f8* owner;
    u8 pad_0xb8[0x1d8 - 0xb8];
    f32 field_0x1d8;
    f32 field_0x1dc;
    u8 pad_0x1e0[0x4];
    f32 field_0x1e4;
    u8 pad_0x1e8[0xc];
    f32 field_0x1f4;
};

// One probe: from a point to a point, and where and with which normal it hit.
struct UnkBoundaryRay_l7659f8 {
    CVector2D from;
    CVector2D to;
    CVector2D hitPos;
    CVector2D hitNormal;
    UnkBoundaryRay_l7659f8(f32 x0, f32 y0, f32 x1, f32 y1)
        : from{x0, y0}, to{x1, y1}, hitPos{g_unk_7101647780.x, g_unk_7101647780.y},
          hitNormal{g_unk_7101647780.x, g_unk_7101647780.y} {}
};

// The segment handed to the shape query.
struct UnkRayBox_l7659f8 {
    CVector2D from;
    CVector2D to;
    CVector2D dir;
};

// The query's hit record (vtable at 0x7101a22c60).
struct UnkRayHit_l7659f8 {
    void* vtable;
    s32 field_0x8;
    CVector3D pos;
    CVector3D field_0x18;
    CVector3D normal;
    u32 field_0x30;
    u32 field_0x34;
    UnkRayHit_l7659f8()
        : vtable(g_unk_7101a22c60 + 0x10), field_0x8(-1), pos(), field_0x18(), normal(), field_0x30(0), field_0x34(0) {}
};

static inline UnkShapeTester_A5_b68* GetTester_l7659f8() {
    if (!g_unk_71726a47a0) {
        UnkShapeTester_A5_b68* t = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
        fn_7100d5134c(t);
        g_unk_71726a47a0 = t;
    }
    return g_unk_71726a47a0;
}

static inline f32 Abs(f32 v) {
    return __builtin_fabsf(v);
}

// Everything a probe needs: the shared tester, the shape and its placement, and the query's records.
struct UnkRayQuery_l7659f8 {
    UnkShapeTester_A5_b68* tester;
    UnkShape* shape;
    const UnkMatrix44* placement;
    UnkRayBox_l7659f8* box;
    UnkRayHit_l7659f8* hit;
};

inline bool CastRay_l7659f8(const UnkRayQuery_l7659f8& q, const UnkBoundaryRay_l7659f8& ray) {
    q.box->from = ray.from;
    q.box->to = ray.to;
    q.box->dir.x = ray.to.x - ray.from.x;
    q.box->dir.y = ray.to.y - ray.from.y;
    return fn_7100d56604(reinterpret_cast<UnkWorld*>(q.tester), reinterpret_cast<const UnkQueryBox*>(q.box), q.shape,
                         reinterpret_cast<const UnkMtx34*>(q.placement), q.hit, 2)
               .bits &
           1;
}

// A vertical ray: how far its hit lies from its end, in y.
inline bool CastVertical_l7659f8(const UnkRayQuery_l7659f8& q, UnkBoundaryRay_l7659f8& ray, f32& offset) {
    if (!CastRay_l7659f8(q, ray))
        return false;
    fn_710007b444(&ray.hitPos, reinterpret_cast<const UnkVec3*>(&q.hit->pos));
    fn_710007b444(&ray.hitNormal, reinterpret_cast<const UnkVec3*>(&q.hit->normal));
    offset = ray.hitPos.y - ray.to.y;
    return true;
}

// A horizontal ray: only a hit on a wall (a mostly horizontal normal) counts; how far it lies from its end, in x.
inline bool CastHorizontal_l7659f8(const UnkRayQuery_l7659f8& q, UnkBoundaryRay_l7659f8& ray, f32& offset) {
    if (!CastRay_l7659f8(q, ray))
        return false;
    fn_710007b444(&ray.hitNormal, reinterpret_cast<const UnkVec3*>(&q.hit->normal));
    if (Abs(ray.hitNormal.x) < Abs(ray.hitNormal.y))
        return false;
    fn_710007b444(&ray.hitPos, reinterpret_cast<const UnkVec3*>(&q.hit->pos));
    offset = ray.hitPos.x - ray.to.x;
    return true;
}

// A horizontal ray moved to the given height.
inline bool CastAtHeight_l7659f8(const UnkRayQuery_l7659f8& q, UnkBoundaryRay_l7659f8& ray, f32 y, f32& offset) {
    ray.from.y = y;
    ray.to.y = y;
    return CastHorizontal_l7659f8(q, ray, offset);
}

void fn_71007659f8(UnkCameraBoundary_l7659f8* self, CVector3D* out, s32 axes) {
    const f32 x0 = self->field_0x1d8;
    const f32 y0 = self->field_0x1dc;
    const f32 x1 = self->field_0x1e4;
    const f32 y1 = self->field_0x1f4;
    const f32 midY = (y0 + y1) * 0.5f;
    f32 offsets[8] = {};
    UnkBoundaryRay_l7659f8 rays[8] = {
        {x1, y1, x1, y0}, {x0, y1, x0, y0}, {x1, y0, x1, y1}, {x0, y0, x0, y1},
        {x0, y0, x1, y0}, {x0, y1, x1, y1}, {x1, y0, x0, y0}, {x1, y1, x0, y1},
    };
    UnkShape* shape = self->owner->entity->collider->shape->collection;
    UnkShapeTester_A5_b68* tester = GetTester_l7659f8();
    UnkBoundaryCollider_l7659f8* collider = self->owner->entity->collider;
    UnkMatrix44 m = fn_7100d36a2c(reinterpret_cast<UnkShape*>(collider ? collider->shape : nullptr));
    UnkRayHit_l7659f8 hit;
    UnkRayBox_l7659f8 box = {};
    const UnkRayQuery_l7659f8 q = {tester, shape, &m, &box, &hit};
    u8 hits = 0;

    if (axes == 1 || axes == 3) {
        if (CastVertical_l7659f8(q, rays[3], offsets[3]))
            hits |= 8;
        if (CastVertical_l7659f8(q, rays[2], offsets[2]))
            hits |= 4;
        const f32 maxGap = (y0 - y1) * 0.675f;
        if (CastVertical_l7659f8(q, rays[1], offsets[1]))
            hits |= 2;
        if (CastVertical_l7659f8(q, rays[0], offsets[0]))
            hits |= 1;
        if (Abs(offsets[0] - offsets[1]) > maxGap) {
            if (offsets[0] > offsets[1]) {
                hits &= ~2;
                offsets[1] = 0.0f;
            } else {
                hits &= ~1;
                offsets[0] = 0.0f;
            }
        }
        if (Abs(offsets[2] - offsets[3]) > maxGap) {
            if (offsets[2] < offsets[3]) {
                offsets[3] = 0.0f;
                hits &= ~8;
            } else {
                hits &= ~4;
                offsets[2] = 0.0f;
            }
        }
    }

    if (axes == 2 || axes == 3) {
        if (CastHorizontal_l7659f8(q, rays[7], offsets[7]))
            hits |= 0x80;
        if (CastHorizontal_l7659f8(q, rays[6], offsets[6]))
            hits |= 0x40;
        const f32 maxGap = (x0 - x1) * 0.5f;
        if (CastHorizontal_l7659f8(q, rays[5], offsets[5]))
            hits |= 0x20;
        if (CastHorizontal_l7659f8(q, rays[4], offsets[4]))
            hits |= 0x10;
        if (Abs(offsets[4] - offsets[5]) > maxGap) {
            if (hits != 0x18 && !(hits & 8) && offsets[4] > offsets[5]) {
                offsets[4] = 0.0f;
                hits &= ~0x10;
                if (CastAtHeight_l7659f8(q, rays[4], midY, offsets[4]))
                    hits |= 0x10;
            } else if (hits != 0x22 && !(hits & 2)) {
                offsets[5] = 0.0f;
                hits &= ~0x20;
                if (CastAtHeight_l7659f8(q, rays[5], midY, offsets[5]))
                    hits |= 0x20;
            }
        }
        if (Abs(offsets[6] - offsets[7]) > maxGap) {
            if (hits != 0x81 && !(hits & 2) && offsets[6] > offsets[7]) {
                offsets[7] = 0.0f;
                hits &= ~0x80;
                if (CastAtHeight_l7659f8(q, rays[7], midY, offsets[7]))
                    hits |= 0x80;
            } else if (hits != 0x44 && !(hits & 4)) {
                offsets[6] = 0.0f;
                hits &= ~0x40;
                if (CastAtHeight_l7659f8(q, rays[6], midY, offsets[6]))
                    hits |= 0x40;
            }
        }
        if (hits < 0x10) {
            if (CastAtHeight_l7659f8(q, rays[7], midY, offsets[7]))
                hits |= 0x80;
            if (CastAtHeight_l7659f8(q, rays[6], midY, offsets[6]))
                hits |= 0x40;
            if (CastAtHeight_l7659f8(q, rays[5], midY, offsets[5]))
                hits |= 0x20;
            if (CastAtHeight_l7659f8(q, rays[4], midY, offsets[4]))
                hits |= 0x10;
        }
    }

    *out = g_unk_7101647790;
    switch (hits) {
    case 0:
        break;
    case 0x11:
    case 0x13:
        if (Abs(rays[4].hitNormal.x) >= Abs(rays[0].hitNormal.y) &&
            (Abs(rays[0].hitNormal.x) > Abs(rays[0].hitNormal.y) || rays[0].hitPos.y < midY))
            out->x = offsets[4];
        else
            out->y = offsets[0];
        break;
    case 0x24:
    case 0x2c:
        if (Abs(rays[5].hitNormal.x) < Abs(rays[2].hitNormal.y))
            out->y = offsets[2];
        else
            out->x = offsets[5];
        break;
    case 0x42:
    case 0x43:
        if (Abs(rays[6].hitNormal.x) >= Abs(rays[1].hitNormal.y) &&
            (Abs(rays[1].hitNormal.x) > Abs(rays[1].hitNormal.y) || rays[1].hitPos.y < midY))
            out->x = offsets[6];
        else
            out->y = offsets[1];
        break;
    case 0x88:
    case 0x8c:
        if (Abs(rays[7].hitNormal.x) < Abs(rays[3].hitNormal.y))
            out->y = offsets[3];
        else
            out->x = offsets[7];
        break;
    default:
        out->x = (offsets[4] > offsets[5] ? offsets[4] : offsets[5]) + (offsets[6] < offsets[7] ? offsets[6] : offsets[7]);
        out->y = (offsets[0] < offsets[1] ? offsets[0] : offsets[1]) + (offsets[2] > offsets[3] ? offsets[2] : offsets[3]);
        break;
    }
}
