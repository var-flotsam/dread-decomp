// CSluggerSpitAttack unit: while the owner is busy, or its AI's timer is below the attack's limit, aims with
// the owner's angles, turns the angle at +0xd4 toward +0xd0 by at most 15 degrees, hands it to the owner's
// property bag, recomputes the aim when not turning and with no live track, then applies it (vtable 0xe8).
// Names and layouts are placeholders.
#include "../../game_s7/UnkPropBagS7.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2523.h"
#include "game/UnkXformNode.h"
#include "types.h"

extern const base::global::CStrId g_unk_7172707bc0 __attribute__((visibility("hidden")));

struct UnkSluggerBagHolder_l6c3628 {
    u8 pad_0x0[0x80];
    unk_prop_bag::UnkPropBag bag;
};

struct UnkSluggerAI_l6c3628 {
    u8 pad_0x0[0x14f8];
    f32 timer;  // 0x14f8
};

struct UnkSluggerOwner_l6c3628 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x220 - 0x94];
    UnkSluggerBagHolder_l6c3628* holder;  // 0x220
    u8 pad_0x228[0x18];
    UnkSluggerAI_l6c3628* ai;  // 0x240
};

struct UnkSluggerParams_l6c3628 {
    u8 pad_0x0[0x12c];
    f32 time_limit;  // 0x12c
};

struct UnkSluggerTrackLink_l6c3628 {
    u8 pad_0x0[0x10];
    void* target;  // 0x10
};

struct UnkSluggerSpitAttack_l6c3628;
struct UnkSluggerSpitAttackVtable_l6c3628 {
    u8 pad_0x0[0xe8];
    void (*Apply)(UnkSluggerSpitAttack_l6c3628* self, UnkVec3f* position, UnkVec3f* direction, f32 a, f32 b,
                  UnkVec3f* out);
};

struct UnkSluggerSpitAttack_l6c3628 {
    UnkSluggerSpitAttackVtable_l6c3628* vtable;
    UnkSluggerOwner_l6c3628* owner;  // 0x8
    u8 pad_0x10[0x18];
    UnkSluggerParams_l6c3628* params;  // 0x28
    u8 pad_0x30[0x2c];
    UnkVec3f field_0x5c;
    UnkVec3f field_0x68;
    UnkVec3f field_0x74;
    bool turning;  // 0x80
    u8 pad_0x81[7];
    UnkSluggerTrackLink_l6c3628* track;  // 0x88
    u8 pad_0x90[0x40];
    f32 target_angle;  // 0xd0
    f32 angle;         // 0xd4
    f32 field_0xd8;
    f32 field_0xdc;
    UnkVec3f field_0xe0;
};
static_assert(__builtin_offsetof(UnkSluggerSpitAttack_l6c3628, target_angle) == 0xd0, "target_angle");
static_assert(__builtin_offsetof(UnkSluggerSpitAttack_l6c3628, field_0xe0) == 0xe0, "field_0xe0");

struct UnkActor;
struct UnkSluggerSpitAttack_b46a2;
void fn_71006c0e38(UnkSluggerSpitAttack_b46a2* self, bool mode, UnkVec3f* angles);
UnkVec3f fn_71006c12e8(UnkSluggerSpitAttack_l6c3628* self, const UnkVec3f& angles);
void fn_71006c20cc(UnkSluggerSpitAttack_l6c3628* self, const UnkVec3f& angles, UnkVec3f* direction, f32* target_angle,
                   f32* a, f32* b);

// The owner aims while busy, or while its AI's timer is below the attack's limit.
inline bool CanAim(UnkSluggerSpitAttack_l6c3628* self) {
    if (fn_7100d7bea0(reinterpret_cast<UnkActor*>(self->owner), 1))
        return true;
    UnkSluggerAI_l6c3628* ai = self->owner->ai;
    return ai && ai->timer < self->params->time_limit;
}

void fn_71006c3628(UnkSluggerSpitAttack_l6c3628* self) {
    if (!CanAim(self))
        return;
    UnkSluggerOwner_l6c3628*& owner = self->owner;
    UnkVec3f angles = owner->xform.GetAngle();
    fn_71006c0e38(reinterpret_cast<UnkSluggerSpitAttack_b46a2*>(self), 1, &angles);
    if (self->turning) {
        const f32 max_turn = 0.2617994f;
        f32 target = self->target_angle;
        f32 angle = self->angle;
        f32 lo = angle + -max_turn;
        f32 hi = angle + max_turn;
        f32 down = target > lo ? target : lo;
        f32 up = target < hi ? target : hi;
        self->angle = target > angle ? up : down;
    }
    if (owner && owner->holder) {
        unk_prop_bag::fn_710033187c(&owner->holder->bag, g_unk_7172707bc0,
                                    (f32)((f64)(self->angle * 180.0f) / 3.141592653589793));
    }
    if (!self->turning && !(self->track && self->track->target)) {
        self->field_0x5c = fn_71006c12e8(self, owner->xform.GetAngle());
        fn_71006c20cc(self, owner->xform.GetAngle(), &self->field_0x74, &self->target_angle, &self->field_0xd8, &self->field_0xdc);
    }
    self->vtable->Apply(self, &self->field_0x5c, &self->field_0x74, self->field_0xd8, self->field_0xdc,
                        &self->field_0xe0);
}
