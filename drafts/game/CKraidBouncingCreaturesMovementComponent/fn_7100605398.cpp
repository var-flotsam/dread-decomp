// CKraidBouncingCreaturesMovementComponent's unit (tools/tu_map.py): the update. After the base update
// (fn_7100ac3a78), while the creature is moving (+0x5dc or the stage at +0x5e0 set) it turns the owner's z angle
// by its speed (+0x4b8) times the definition's rate, then tints the actor's slots: the stage and the phase at
// +0x5ec pick the colour, which fades in with +0x5dc and ends at white. Names and layouts are placeholders.
#include <math.h>
#include "game/CProjectileMovement.h"
#include "game/UnkTU53.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkKraidSlots_l605398;
struct UnkKraidColour_l605398 {
    f32 x, y, z;
};
void fn_71001435f0(UnkKraidSlots_l605398* slots, const UnkKraidColour_l605398* colour);

struct UnkKraidSlotsOwner_l605398 {
    u8 pad_0x0[0x50];
    UnkKraidSlots_l605398* slots;  // 0x50
};

struct UnkKraidOwner_l605398 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x228 - 0x94];
    UnkKraidSlotsOwner_l605398* field_0x228;
    u8 pad_0x230[0x3c0 - 0x230];
    bool field_0x3c0;
};
static_assert(__builtin_offsetof(UnkKraidOwner_l605398, field_0x3c0) == 0x3c0, "layout");

struct UnkKraidDef_l605398 {
    u8 pad_0x0[0x134];
    f32 turn_rate;  // 0x134
};

struct UnkKraidBouncing_l605398 {
    u8 pad_0x0[0x30];
    UnkKraidOwner_l605398* owner;  // 0x30
    u8 pad_0x38[0x4b8 - 0x38];
    f32 velocity_x;
    f32 velocity_y;
    f32 velocity_z;
    u8 pad_0x4c4[0x5d0 - 0x4c4];
    UnkKraidDef_l605398* def;  // 0x5d0
    f32 field_0x5d8;
    f32 fade;     // 0x5dc
    s32 stage;    // 0x5e0
    u8 pad_0x5e4[0x5ec - 0x5e4];
    f32 phase;    // 0x5ec
    u8 pad_0x5f0[0x4];
    f32 field_0x5f4;
    f32 field_0x5f8;
};
static_assert(__builtin_offsetof(UnkKraidBouncing_l605398, fade) == 0x5dc, "layout");

void fn_7100605398(UnkKraidBouncing_l605398* self, f32 dt) {
    fn_7100ac3a78(reinterpret_cast<UnkNailongThornMovement*>(self), dt);
    if (self->fade != 0.0f || self->stage != 0) {
        UnkVec3f angle = self->owner->xform.GetAngle();
        f32 speed = sqrtf(self->velocity_x * self->velocity_x + self->velocity_y * self->velocity_y +
                          self->velocity_z * self->velocity_z);
        f32 share;
        if (!(speed > 0.0f))
            share = 0.0f;
        else
            share = (!(speed >= 1500.0f) ? speed : 1500.0f) / 1500.0f;
        f32 turn = self->def->turn_rate * (dt * 6.2831855f * share);
        f32 delta = self->velocity_x < 0.0f ? turn : -turn;
        self->owner->xform.SetAngle(UnkVec3f(angle.x, angle.y, delta + angle.z));
        self->owner->field_0x3c0 = true;
    }
    UnkKraidSlotsOwner_l605398* holder = self->owner->field_0x228;
    if (!holder)
        return;
    UnkKraidSlots_l605398* slots = holder->slots;
    if (!slots)
        return;
    bool near_one = self->phase + -0.001f <= 1.0f && self->phase + 0.001f >= 1.0f;
    s32 stage = self->stage;
    bool grow = near_one ? stage > 1 : stage >= 1;
    f32 fade = self->fade;
    if (grow && fade < 1.0f) {
        fade = dt * 6.0f + fade;
        self->fade = fade;
        if (fade >= 1.0f) {
            self->fade = 1.0f;
            fade = 1.0f;
            goto white;
        }
    }
    if (fade >= 1.0f)
        goto white;
    UnkKraidColour_l605398 c1, c2, c3, d0, d1, d2;
    if (near_one) {
        if (stage == 0) {
            UnkKraidColour_l605398 c0;
            c0.x = 1.0f;
            c0.y = 0.6f;
            c0.z = 1.0f;
            fn_71001435f0(slots, &c0);
        } else
        switch (stage) {
        case 1:
            self->velocity_x = self->field_0x5f4;
            self->velocity_y = self->field_0x5d8 > 0.0f ? self->field_0x5d8 : -self->field_0x5f8;
            c1.x = 1.0f;
            c1.y = 0.4f;
            c1.z = 1.0f;
            fn_71001435f0(slots, &c1);
            break;
        case 2:
            c2.x = 0.5f;
            c2.y = 1.0f;
            c2.z = 1.0f;
            fn_71001435f0(slots, &c2);
            break;
        default:
            c3.x = fn_7100076020(fade, "s") * 0.5f + 0.5f;
            c3.y = 1.0f;
            c3.z = 1.0f;
            fn_71001435f0(slots, &c3);
            break;
        }
    } else {
        switch (stage) {
        case 0:
            d0.x = 1.0f;
            d0.y = 0.4f;
            d0.z = 1.0f;
            fn_71001435f0(slots, &d0);
            break;
        case 1:
            d1.x = 0.5f;
            d1.y = 1.0f;
            d1.z = 1.0f;
            fn_71001435f0(slots, &d1);
            break;
        default:
            d2.x = fn_7100076020(fade, "s") * 0.5f + 0.5f;
            d2.y = 1.0f;
            d2.z = 1.0f;
            fn_71001435f0(slots, &d2);
            break;
        }
    }
    ++self->stage;
    return;
white : {
    UnkKraidColour_l605398 white;
    white.x = 1.0f;
    white.y = 1.0f;
    white.z = 1.0f;
    fn_71001435f0(slots, &white);
}
}
