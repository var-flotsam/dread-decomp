// CAbilityGhostAura unit: picks the side the aura leans to from the owner's aim (the actor's object at +0x1d8)
// and the ability's float at +0x3f0. The state (0 none, 1 or 2) is kept in *state: it is chosen when the aim's
// angle lies between the two aim tunables and its direction is off centre, and the output direction is
// (+-sign, 0, 0) for it. Names and layouts are placeholders.
#include <math.h>

#include "game/UnkTU437.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkAimTunable {
    u8 pad_0x0[0xa8];
    f32 field_0xa8;
    u8 pad_0xac[0xb8 - 0xac];
    f32 field_0xb8;
};

struct UnkAuraAim_l30c1c0 {
    u8 pad_0x0[0x9ec];
    UnkVec3f dir;
};

struct UnkAuraActor_l30c1c0 {
    u8 pad_0x0[0x1d8];
    UnkAuraAim_l30c1c0* aim;
};

struct UnkAuraOwner_l30c1c0 {
    u8 pad_0x0[0x30];
    UnkAuraActor_l30c1c0* actor;
};

struct UnkSelf {
    u8 pad_0x0[0xc0];
    UnkAuraOwner_l30c1c0* owner;
    u8 pad_0xc8[0x3f0 - 0xc8];
    f32 field_0x3f0;
};

struct UnkAuraSide_l30c1c0 {
    f32 x;
    f32 y;
    f32 z;
};

static inline bool NearZero_l30c1c0(f32 v) {
    return v - 0.001f <= 0.0f && v + 0.001f >= 0.0f;
}

static inline f32 Sign_l30c1c0(f32 v) {
    return (v > 0.0f) - (v < 0.0f);
}

void fn_710030c1c0(UnkSelf* self, u32* state, UnkAuraSide_l30c1c0* out) {
    *state = 0;
    if (self->owner == nullptr || self->owner->actor == nullptr || self->owner->actor->aim == nullptr)
        return;
    UnkAuraAim_l30c1c0* aim = self->owner->actor->aim;
    UnkVec3f dir = aim->dir;
    f32 dir_x = dir.x;
    f32 angle = NearZero_l30c1c0(aim->dir.z) ? 0.0f : atan2f(aim->dir.y, fabsf(dir_x));
    UnkAimTunable* tunable = &g_unk_71726b1438;
    if (angle >= tunable->field_0xb8 && !NearZero_l30c1c0(dir_x) && angle <= tunable->field_0xa8) {
        f32 t = self->field_0x3f0;
        if (dir_x > 0.0f && t > 0.0f)
            *state = 1;
        else if (t < 0.0f && dir_x < 0.0f)
            *state = 1;
        else
            *state = 2;
    }
    switch (*state) {
    case 0:
        out->x = 0.0f;
        out->y = 0.0f;
        out->z = 0.0f;
        break;
    case 1:
        out->x = Sign_l30c1c0(self->field_0x3f0);
        out->y = 0.0f;
        out->z = 0.0f;
        break;
    case 2:
        out->x = -Sign_l30c1c0(self->field_0x3f0);
        out->y = 0.0f;
        out->z = 0.0f;
        break;
    }
}
