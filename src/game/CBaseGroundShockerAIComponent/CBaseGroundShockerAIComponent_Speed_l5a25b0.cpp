// CBaseGroundShockerAIComponent's unit: after the base routine (fn_7101111c24) fills `out`, while the
// movement allows it and the speed is not about zero, turns both speed vectors (+0x60, +0x6c) to point
// along or against the surface's direction, keeping their length. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVectorOps.h"
#include "game/CEnemyMovement.h"
#include "game/UnkTU2702.h"
#include "types.h"

using base::math::CVector3D;

struct UnkCaterzillaAIScale_b49;
struct UnkCaterzillaOut_b49;

struct UnkShockerSurface_l5a25b0 {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
    u8 pad_0x14[0xd0];
    f32 dir_x;  // 0xe4
    f32 dir_y;  // 0xe8
};

struct UnkShockerContact_l5a25b0 {
    u8 pad_0x0[0x10];
    UnkShockerSurface_l5a25b0* surface;  // 0x10
};

struct UnkShockerCollision_l5a25b0 {
    u8 pad_0x0[0xb0];
    UnkShockerContact_l5a25b0** contact;  // 0xb0
};

struct UnkShockerOwner_l5a25b0 {
    u8 pad_0x0[0x218];
    void* movement;  // 0x218
    u8 pad_0x220[0x78];
    UnkShockerCollision_l5a25b0* collision;  // 0x298
};

struct UnkShocker_l5a25b0 {
    u8 pad_0x0[0x30];
    UnkShockerOwner_l5a25b0* owner;  // 0x30
};

struct UnkShockerOut_l5a25b0 {
    u8 pad_0x0[0x60];
    CVector3D a;  // 0x60
    CVector3D b;  // 0x6c
};

// Whether v is outside [-0.001, 0.001] (or not a number).
static inline bool NotNearZero(f32 v) {
    return !(v + 0.001f >= 0.0f) || !(v - 0.001f <= 0.0f);
}

void fn_71005a25b0(UnkShocker_l5a25b0* self, UnkShockerOut_l5a25b0* out) {
    fn_7101111c24(reinterpret_cast<UnkCaterzillaAIScale_b49*>(self), reinterpret_cast<UnkCaterzillaOut_b49*>(out));
    void* movement = self->owner->movement;
    if (!movement || !fn_71010cab0c(movement))
        return;
    if (!NotNearZero(out->a.x) && !NotNearZero(out->a.y) && !NotNearZero(out->a.z))
        return;
    UnkShockerContact_l5a25b0* contact = *self->owner->collision->contact;
    if (!contact)
        return;
    UnkShockerSurface_l5a25b0* surface = contact->surface;
    if (!surface || surface->kind != 2)
        return;
    CVector3D dir = base::math::Normalized(CVector3D{-surface->dir_x, -surface->dir_y, 0.0f});
    CVector3D speed = out->a;
    f32 length = base::math::Length(speed);
    CVector3D unit = speed * (1.0f / length);
    f32 dot = base::math::Dot(unit, dir);
    f32 sign = (f32)((dot > 0.0f) - (dot < 0.0f));
    CVector3D turned = dir * sign * length;
    out->a = turned;
    out->b = turned;
}
