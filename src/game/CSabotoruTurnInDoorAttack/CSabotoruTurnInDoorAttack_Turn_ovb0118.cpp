// CSabotoruTurnInDoorAttack's unit (tools/tu_map.py). fn_7100673fd8: a record's copy assignment, the
// same code as CInfesterShootAttack's fn_71005c7640 (plain fields, then the weak reference at +0x50
// unless assigned to itself, then the word at +0x58). fn_7100673908: picks the animation to play
// (fn_71006737fc) from the owner's transform: facing up with no sideways component, sideways, or
// neither. fn_7100673a18: steps the turn: states 1 and 2 go to 3 and 1 (fn_710067342c); in state 3,
// a CSabotoruAIComponent owner whose door turn (+0x146d8) is not running starts it on the owner's
// animation (+0x1b8, slots 0x170 and 0x168); otherwise the attack goes to state 4 and plays its end.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkSabotoruOwner_ovb0118 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
};

struct UnkSabotoruAttack_ovb0118 {
    u8 pad_0x0[0x8];
    UnkSabotoruOwner_ovb0118* owner;  // 0x8
};

void fn_71006737fc(UnkSabotoruAttack_ovb0118* self, u64 animation);

// Whether `v` is within 0.05 of `target`.
static inline bool IsNear(f32 v, f32 target) {
    return v - 0.05f <= target && v + 0.05f >= target;
}

void fn_7100673908(UnkSabotoruAttack_ovb0118* self) {
    const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
    f32 side = m.m[1];
    f32 up = m.m[5];
    if (up > 0.0f && IsNear(side, 0.0f))
        return fn_71006737fc(self, 0xc2d1d284f785eda6);
    if ((IsNear(side, -1.0f) || IsNear(side, 1.0f)) && IsNear(up, 0.0f))
        return fn_71006737fc(self, 0x24ed25e5223bca66);
    return fn_71006737fc(self, 0xf0cc32abb15c788a);
}

struct UnkSabotoruDoorTurn_ovb0118 {
    f32 speed;  // 0x0
    u8 pad_0x4[0x9c];
    s32 state;  // 0xa0
};

struct UnkSabotoruAI_ovb0118 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x146d0];
    UnkSabotoruDoorTurn_ovb0118 turn;  // 0x146d8
};

struct UnkSabotoruAnim_ovb0118 {
    void** vtable;
    u8 pad_0x8[0x48];
    f32 field_0x50;

    void vf_0x168(s32 a, f32 b) {
        reinterpret_cast<void (*)(UnkSabotoruAnim_ovb0118*, s32, f32)>(vtable[0x168 / 8])(this, a, b);
    }
    void vf_0x170(f32 v) {
        reinterpret_cast<void (*)(UnkSabotoruAnim_ovb0118*, f32)>(vtable[0x170 / 8])(this, v);
    }
};

struct UnkSabotoruOwner2_ovb0118 {
    u8 pad_0x0[0x1b8];
    UnkSabotoruAnim_ovb0118* anim;  // 0x1b8
    u8 pad_0x1c0[0x80];
    UnkSabotoruAI_ovb0118* ai;  // 0x240
};

struct UnkSabotoruAttack2_ovb0118 {
    u8 pad_0x0[0x8];
    UnkSabotoruOwner2_ovb0118* owner;  // 0x8
    u8 pad_0x10[0x20];
    s32 state;  // 0x30
};

struct UnkSabotoruAttack_A3_b73;
void fn_710067342c(UnkSabotoruAttack_A3_b73* self, bool value);

// fn_710067342c takes a state (1 or 3 here), declared elsewhere with a bool.
static inline void SetTurnState(UnkSabotoruAttack2_ovb0118* self, s32 state) {
    reinterpret_cast<void (*)(UnkSabotoruAttack2_ovb0118*, s32)>(fn_710067342c)(self, state);
}

void fn_7100673a18(UnkSabotoruAttack2_ovb0118* self) {
    switch (self->state) {
    case 1:
        return SetTurnState(self, 3);
    case 2:
        return SetTurnState(self, 1);
    case 3: {
        UnkSabotoruAI_ovb0118* ai = self->owner->ai;
        if (ai && fn_710009d50c(ai->vf_0x0(), fn_7100677dd4()) && ai->turn.state != 1) {
            if (!self->owner->anim)
                return;
            // Stored untyped: the original reloads the owner's animation after it.
            s32 turning = 1;
            __builtin_memcpy(&ai->turn.state, &turning, sizeof(turning));
            self->owner->anim->vf_0x170(ai->turn.speed);
            UnkSabotoruAnim_ovb0118* anim = self->owner->anim;
            return anim->vf_0x168(1, anim->field_0x50);
        }
        if (self->state != 4) {
            self->state = 4;
            return fn_71006737fc(reinterpret_cast<UnkSabotoruAttack_ovb0118*>(self), 0xf3714ec0d2522f3b);
        }
        return;
    }
    }
}

struct UnkSabotoruRecord_ovb0118 {
    u8 pad_0x0[0x8];
    u8 field_0x8[32];
    u64 field_0x28;
    u32 field_0x30;
    u8 pad_0x34[0xc];
    u8 field_0x40[16];
    UnkWeakHandle* field_0x50;
    u64 field_0x58;
};

UnkSabotoruRecord_ovb0118* fn_7100673fd8(UnkSabotoruRecord_ovb0118* self, const UnkSabotoruRecord_ovb0118* other) {
    __builtin_memcpy(self->field_0x8, other->field_0x8, 32);
    u64 field_0x28 = other->field_0x28;
    u32 field_0x30 = other->field_0x30;
    self->field_0x30 = field_0x30;
    self->field_0x28 = field_0x28;
    __builtin_memcpy(self->field_0x40, other->field_0x40, 16);
    if (self != other)
        CopyWeakRef(&self->field_0x50, other->field_0x50);
    self->field_0x58 = other->field_0x58;
    return self;
}
