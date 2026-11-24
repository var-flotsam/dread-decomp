// Unit 437 (tools/tu_map.py, CAimComponent by its hint): which aim mode the owner is in, from a forced-aim
// track, the aim angle against the tuned limits and the gun's state; sets the two out-flags on the way.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CGunComponent.h"
#include "game/CHyperBeamGun.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU39.h"
#include "game/UnkTU437.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkGunComponent;
struct UnkComponent;
struct UnkSamusGunComponent;

bool fn_71009e9250(void* body, u32 mode);

// The aim limits (angles) of g_unk_71726b1438.
struct UnkAimLimits_l34e490 {
    u8 pad_0x0[0xa4];
    f32 up_ground;      // 0xa4
    f32 up;             // 0xa8
    f32 up_diagonal;    // 0xac
    f32 down_ground;    // 0xb0
    f32 down_diagonal;  // 0xb4
    f32 down;           // 0xb8
    f32 forward_up;     // 0xbc
    f32 forward_down;   // 0xc0
};

static inline const UnkAimLimits_l34e490& AimLimits() {
    return reinterpret_cast<const UnkAimLimits_l34e490&>(g_unk_71726b1438);
}

struct UnkAimTrack_l34e490 {
    u8 pad_0x0[0x18];
    u32 mode;  // 0x18
};

struct UnkAimBody_l34e490;

struct UnkAimBodyVtbl_l34e490 {
    u8 pad_0x0[0x3b8];
    bool (*vf_0x3b8)(UnkAimBody_l34e490* self);
};

struct UnkAimBody_l34e490 {
    UnkAimBodyVtbl_l34e490* vtable;
    u8 pad_0x8[0x7c - 0x8];
    f32 field_0x7c;
    u8 pad_0x80[0xf9 - 0x80];
    bool field_0xf9;
    u8 pad_0xfa[0x3a8 - 0xfa];
    bool field_0x3a8;
    u8 pad_0x3a9[0x7e4 - 0x3a9];
    bool field_0x7e4;
};

struct UnkAimGun_l34e490 {
    u8 pad_0x0[0x178];
    bool busy;  // 0x178
};

struct UnkAimGrapple_l34e490 {
    u8 pad_0x0[0x10];
    u32 field_0x10;
};

struct UnkAimGrappleSub_l34e490 {
    u8 pad_0x0[0x490];
    UnkAimGrapple_l34e490* field_0x490;
};

struct UnkAimGrappleHolder_l34e490 {
    u8 pad_0x0[0x10];
    UnkAimGrappleSub_l34e490* field_0x10;
};

struct UnkAimCurrentGun_l34e490 {
    u8 pad_0x0[0x20];
    base::global::CStrId id;  // 0x20
    u8 pad_0x28[0x1888 - 0x28];
    UnkAimGrappleHolder_l34e490* field_0x1888;
};

struct UnkAimOwner_l34e490 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x208 - 0x78];
    UnkAimBody_l34e490* body;  // 0x208
    u8 pad_0x210[0x220 - 0x210];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x2a8 - 0x228];
    UnkAimGun_l34e490* gun;  // 0x2a8
};

struct UnkAimState_l34e490 {
    u8 pad_0x0[0x10];
    u32 field_0x10;
};

struct UnkAimRanges_l34e490 {
    u8 pad_0x0[0x80];
    f32 near;      // 0x80
    f32 near_air;  // 0x84
};

struct UnkAimComponent_l34e490 {
    u8 pad_0x0[0x30];
    UnkAimOwner_l34e490* owner;  // 0x30
    u8 pad_0x38[0x50 - 0x38];
    UnkAimState_l34e490* state;  // 0x50
    u8 pad_0x58[0x2b0 - 0x58];
    UnkAimRanges_l34e490* ranges;  // 0x2b0
    u8 pad_0x2b8[0x330 - 0x2b8];
    f32 weight_x;  // 0x330
    f32 weight_y;  // 0x334
    u8 pad_0x338[0x4];
    f32 angle;  // 0x33c
    u8 pad_0x340[0x4];
    bool field_0x344;
    u8 pad_0x345[0x358 - 0x345];
    u32 field_0x358;
    u8 pad_0x35c[0x368 - 0x35c];
    u8 list_0x368[0xb0];
    u8 list_0x418[0xb0];
    bool field_0x4c8;
    u8 pad_0x4c9[0x3];
    f32 field_0x4cc;
    f32 field_0x4d0;
    f32 field_0x4d4;
    bool field_0x4d8;
};
static_assert(__builtin_offsetof(UnkAimComponent_l34e490, field_0x4d8) == 0x4d8, "layout");

static inline bool InState2(UnkAimComponent_l34e490* self) {
    return self->state && self->state->field_0x10 == 2;
}

static bool HyperBeamReady(UnkAimGun_l34e490* gun) {
    if (!(*fn_71008407fc(reinterpret_cast<UnkGunComponent*>(gun)) == GameManagerIdTable().field_0x1848))
        return false;
    auto* beam = static_cast<base::reflection::UnkReflObject*>(fn_710083cd30(reinterpret_cast<UnkComponent*>(gun)));
    if (beam && !fn_710009d50c(beam->vf_0x0(), fn_71008424e8()))
        beam = nullptr;
    return fn_7100845fac(beam);
}

static inline bool GrappleBusy(UnkAimGun_l34e490* gun) {
    auto* current = reinterpret_cast<UnkAimCurrentGun_l34e490*>(
        fn_710083cd4c(reinterpret_cast<UnkSamusGunComponent*>(gun)));
    if (!current || !(current->id == GameManagerIdTable().field_0x1240))
        return false;
    if (!fn_710009d50c(reinterpret_cast<base::reflection::UnkReflObject*>(current)->vf_0x0(), fn_7100832034()))
        return false;
    UnkAimGrappleHolder_l34e490* holder = current->field_0x1888;
    if (!holder || !holder->field_0x10)
        return false;
    UnkAimGrapple_l34e490* grapple = holder->field_0x10->field_0x490;
    return !grapple || grapple->field_0x10 != 1;
}

// Whether the target is within reach.
static inline bool IsNear(UnkAimComponent_l34e490* self, UnkAimBody_l34e490* body) {
    f32 distance = self->field_0x4cc;
    if (distance >= 0.0f &&
        (body->field_0x3a8 ? distance < self->ranges->near : distance < self->ranges->near_air))
        return true;
    return self->field_0x358 != 0 && self->field_0x4d0 >= 0.0f && self->field_0x4d0 < self->ranges->near;
}

// The mode, before a zero mode clears `blend`.
static inline u32 AimMode(UnkAimComponent_l34e490* self, bool* locked, bool* blend) {
    UnkAimOwner_l34e490* owner = self->owner;
    UnkAimBody_l34e490* body = owner->body;
    UnkAnimationComponent* animation = owner->animation;
    if (!body || !animation)
        return 0;
    UnkAimGun_l34e490* gun = owner->gun;

    UnkTrackQuery query = {fn_7100cf9bb8(), nullptr, -1.0f};
    if (self->owner->animation) {
        fn_710117b08c(self->owner->animation, &query);
        if (query.result) {
            u32 forced = reinterpret_cast<UnkAimTrack_l34e490*>(query.result)->mode;
            *blend = false;
            return forced;
        }
    }

    bool near = IsNear(self, body);

    bool grounded = fn_71009e12d0(body);
    f32 upper = fn_7100030dec(reinterpret_cast<UnkFloatList_b46a1*>(self->list_0x368));
    f32 lower = fn_7100030dec(reinterpret_cast<UnkFloatList_b46a1*>(self->list_0x418));
    const UnkAimLimits_l34e490& limits = AimLimits();
    u32 mode;
    if (grounded && !InState2(self)) {
        const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
        CVector3D facing = {m.m[2], m.m[6], m.m[10]};
        CVector2D flat;
        fn_710007b430(&flat, &facing);
        f32 dot = flat.x * self->weight_x + flat.y * self->weight_y;
        bool crouched = fn_71009e6434(reinterpret_cast<UnkHelper*>(body));
        f32 angle = self->angle;
        f32 up = crouched ? limits.up_ground : limits.up;
        f32 down = crouched ? limits.down_ground : limits.down;
        if (dot > 0.0f && angle <= up && angle >= down)
            mode = 3;
        else if (angle < down)
            mode = 5;
        else if (angle > up)
            mode = 1;
        else
            mode = 0;
    } else {
        f32 angle = self->angle;
        if (self->field_0x4c8 ? angle < lower : angle < limits.down)
            mode = 5;
        else if (self->field_0x4c8 ? angle > upper : angle > limits.up)
            mode = 1;
        else if (!(self->field_0x344 && !near) && angle > limits.up_diagonal && angle <= limits.up) {
            *locked = true;
            mode = 2;
        } else if (!(self->field_0x344 && !near) && angle < limits.down_diagonal && angle >= limits.down) {
            *locked = true;
            mode = 4;
        } else if ((!self->field_0x344 && near) &&
                   ((angle > limits.forward_up && angle <= limits.up_diagonal) ||
                    (angle < limits.forward_down && angle >= limits.down_diagonal))) {
            *locked = true;
            mode = 3;
        } else if (InState2(self) && ((angle >= 0.0f && angle <= limits.forward_up) ||
                                      (angle < 0.0f && angle >= limits.forward_down))) {
            *locked = true;
            mode = 3;
        } else if ((gun && gun->busy) || fn_7100df59a8(g_unk_7101d47310) ||
                   (gun && fn_710084088c(reinterpret_cast<UnkSamusGunComponent*>(gun))) ||
                   g_unk_7101d47310->field_0xc8 != 0 || g_unk_7101d47310->field_0x10c ||
                   IsOpticCamouflageAbilityInState6(g_unk_7101d47310)) {
            mode = 3;
        } else if (self->field_0x358 != 0 &&
                   (near || self->field_0x4d4 > 0.0f ||
                    (fn_710117b348(animation, fn_7100d038f8()) && !self->field_0x4d8))) {
            *blend = false;
            mode = 3;
        } else {
            mode = 0;
        }
    }

    if (fn_71009e9250(body, mode))
        mode = 3;
    if (mode == 0 && !body->field_0xf9 && !fn_71009e12d0(body) && body->field_0x7e4 && body->field_0x7c > 0.0f)
        mode = 3;
    if (grounded && body->vtable->vf_0x3b8(body) && mode == 5)
        mode = 3;

    if (gun) {
        if (mode == 0 && GrappleBusy(gun))
            mode = 3;
        if (HyperBeamReady(gun))
            return 3;
    }
    return mode;
}

u32 fn_710034e490(UnkAimComponent_l34e490* self, bool* locked, bool* blend) {
    *locked = false;
    *blend = true;
    u32 mode = AimMode(self, locked, blend);
    if (mode == 0)
        *blend = false;
    return mode;
}
