// CShineonAIComponent unit (tools/tu_map.py unit 1058): after the base update, eases the up/down factor at
// +0x146d0 towards the vertical part of the owner's normalized velocity, at most 2*dt per call, and reports
// it to the owner's value bag as "UpDownFactor". Names and layouts are placeholders.
#include <math.h>
#include "../../../src/game_s7/UnkPropBagS7.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkBehaviorTree.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;

struct UnkShineonMovement_l6bef78 {
    u8 pad_0x0[0x40];
    CVector3D velocity;  // 0x40
};

struct UnkShineonBagOwner_l6bef78 {
    u8 pad_0x0[0x80];
    unk_prop_bag::UnkPropBag bag;  // 0x80
};

struct UnkShineonOwner_l6bef78 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x178];
    UnkShineonMovement_l6bef78* movement;  // 0x1f0
    u8 pad_0x1f8[0x28];
    UnkShineonBagOwner_l6bef78* bag_owner;  // 0x220
};
static_assert(__builtin_offsetof(UnkShineonOwner_l6bef78, movement) == 0x1f0, "layout");
static_assert(__builtin_offsetof(UnkShineonOwner_l6bef78, bag_owner) == 0x220, "layout");

struct UnkShineonAI_l6bef78 {
    u8 pad_0x0[0x30];
    UnkShineonOwner_l6bef78* owner;  // 0x30
    u8 pad_0x38[0x14698];
    f32 up_down_factor;  // 0x146d0
};
static_assert(__builtin_offsetof(UnkShineonAI_l6bef78, up_down_factor) == 0x146d0, "layout");

void fn_71006bef78(UnkShineonAI_l6bef78* self, f32 dt) {
    fn_7100369c44(reinterpret_cast<UnkSelf_7100369c44*>(self), dt);
    CVector3D v = self->owner->movement->velocity;
    f32 inv = 1.0f / (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z) + 0.0001f);
    f32 target = v.y * inv;
    fn_710002614c(&self->owner->transform);

    // Move the factor towards the target by at most 2*dt.
    f32& factor = self->up_down_factor;
    f32 step = dt + dt;
    f32 current = factor;
    f32 low = current - step;
    f32 high = step + current;
    f32 down = target > low ? target : low;
    f32 up = target < high ? target : high;
    factor = !(target > current) ? down : up;

    unk_prop_bag::fn_710033187c(&self->owner->bag_owner->bag, CStrId("UpDownFactor", true), factor);
}
