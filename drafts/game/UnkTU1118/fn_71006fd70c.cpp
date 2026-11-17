// Unit 1118: walks the state's list of points, recording the nearest one (in the xy plane) its first
// callback accepts, and stops at the first one within reach that its second callback accepts, turning
// the owner towards it when it lies behind. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/UnkXformNode.h"
#include "types.h"

#include <math.h>

using base::global::CRntVector;
using base::math::CVector2D;
using base::math::CVector3D;

template <class T>
struct UnkCopiedVector;
struct UnkThreeWords;
UnkCopiedVector<UnkThreeWords>* fn_71000cbcf0(UnkCopiedVector<UnkThreeWords>* self,
                                              const UnkCopiedVector<UnkThreeWords>* other);

static inline void Assign(CRntVector<UnkVec3f>* self, const CRntVector<UnkVec3f>* other) {
    fn_71000cbcf0(reinterpret_cast<UnkCopiedVector<UnkThreeWords>*>(self),
                  reinterpret_cast<const UnkCopiedVector<UnkThreeWords>*>(other));
}

// The xy of a point.
static inline void Flat(CVector2D* out, const UnkVec3f& v) {
    fn_710007b430(out, reinterpret_cast<const CVector3D*>(&v));
}

// A float that may be set, with a fallback.
struct UnkOptF32_l6fd70c {
    f32 value;
    bool has;

    const f32& ValueOr(const f32& fallback) const { return has ? value : fallback; }
};

// A callback asked about a point, with the context it is called with.
struct UnkPointCallback_l6fd70c {
    void* context;
    bool (*fn)(void* context, CVector3D* point);
    bool valid;

    bool Call(CVector3D point) const { return fn(context, &point); }
};

struct UnkReachOwner_l6fd70c {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkXform xform;  // 0x38

    // The xy of the owner's position.
    void Position2D(CVector2D* out) {
        UnkVec3f position = xform.GetTranslation();
        Flat(out, position);
    }

    // Turns towards a point.
    void vf_0xd0(const UnkVec3f* point, bool flag) {
        reinterpret_cast<void (*)(UnkReachOwner_l6fd70c*, const UnkVec3f*, bool)>(vtable[0xd0 / 8])(this, point,
                                                                                                    flag);
    }
};

struct UnkReachParams_l6fd70c {
    u8 pad_0x0[0x64];
    f32 reach;  // 0x64
};

struct UnkReachState_l6fd70c {
    u8 pad_0x0[0x30];
    UnkReachOwner_l6fd70c* owner;            // 0x30
    u8 pad_0x38[0x18];
    f32 field_0x50;                          // 0x50
    u8 pad_0x54[0x14];
    UnkReachParams_l6fd70c* params;          // 0x68
    u8 pad_0x70[0x74];
    f32 forward_x;                           // 0xe4
    f32 forward_y;                           // 0xe8
    u8 pad_0xec[0x60];
    UnkOptF32_l6fd70c reach;                 // 0x14c
    u8 pad_0x154[0x54];
    UnkPointCallback_l6fd70c accept;         // 0x1a8
    UnkPointCallback_l6fd70c nearest;        // 0x1c0
    CRntVector<UnkVec3f> points;             // 0x1d8
    bool has_points;                         // 0x200
    u8 pad_0x201[0x12a9f];
    f32 nearest_dist;                        // 0x12ca0
};
static_assert(__builtin_offsetof(UnkReachState_l6fd70c, points) == 0x1d8, "points");
static_assert(__builtin_offsetof(UnkReachState_l6fd70c, nearest_dist) == 0x12ca0, "nearest_dist");

bool fn_71006fd70c(UnkReachState_l6fd70c* self) {
    bool found = false;
    CRntVector<UnkVec3f> points(0xf);
    if (self->has_points)
        Assign(&points, &self->points);
    if (self->has_points && self->nearest.valid && self->accept.valid) {
        const f32& reach = self->reach.ValueOr(self->params->reach);
        f32 reachSq = reach * reach;
        UnkPointCallback_l6fd70c accept = self->accept;
        UnkPointCallback_l6fd70c nearest = self->nearest;
        for (UnkVec3f point : points) {
            CVector2D flat;
            Flat(&flat, point);
            CVector2D ownerFlat;
            self->owner->Position2D(&ownerFlat);
            f32 dx = flat.x - ownerFlat.x;
            f32 dy = flat.y - ownerFlat.y;
            f32 distSq = dx * dx + dy * dy;
            f32 dist = sqrtf(distSq);
            if (nearest.fn && nearest.Call(*reinterpret_cast<CVector3D*>(&point)) && dist < self->nearest_dist)
                self->nearest_dist = dist;
            if (accept.fn && distSq - 0.001f <= reachSq && accept.Call(*reinterpret_cast<CVector3D*>(&point))) {
                if (dx * self->forward_x + dy * self->forward_y < 0.0f) {
                    self->field_0x50 = 0.0f;
                    self->owner->vf_0xd0(&point, true);
                }
                found = true;
                break;
            }
        }
    }
    points.Free();
    return found;
}
