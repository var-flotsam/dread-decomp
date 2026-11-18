// CInfesterShootAttack's unit (tools/tu_map.py unit 887): whether the shot may start. The base check and a
// target are needed, the actor's AI must be a CInfesterAIComponent and its CEnemyMovement in mode 1, the
// AI must find a side to shoot from within reach, and the AI's checks for the current state must pass;
// then the answer is the inverse of fn_71005c6da0. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttack.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkVec2f_A5 {
    f32 x, y;
    UnkVec2f_A5(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkVec2f_A5(const UnkVec2f_A5& o) : x(o.x), y(o.y) {}
};

struct UnkInfesterSelf_l5cc96c;
struct UnkInfesterAI_ovb0130;
UnkVec2f_A5 fn_71005cc96c(UnkInfesterSelf_l5cc96c* self, u32* forward);
bool fn_71005cbcb4(UnkInfesterAI_ovb0130* self);

// The movement's two shot limits (+0x974, +0x978).
struct UnkShotLimits_l5c6b28 {
    s32 a;
    s32 b;
    UnkShotLimits_l5c6b28(const UnkShotLimits_l5c6b28& o) : a(o.a), b(o.b) {}
};

struct UnkInfesterMovement_l5c6b28 {
    u8 pad_0x0[0x974];
    UnkShotLimits_l5c6b28 limits;  // 0x974
};

struct UnkInfesterAIState_l5c6b28 {
    u8 pad_0x0[0x518];
    u32 mode;  // 0x518
};

// The AI's shooting state (+0x146c8).
struct UnkInfesterShot_l5c6b28 {
    UnkInfesterAIState_l5c6b28* state;
    u8 pad_0x8[0xd];
    bool ready;  // 0x15
};

struct UnkInfesterAI_l5c6b28 {
    u8 pad_0x0[0x3f];
    u8 field_0x3f;
    u8 pad_0x40[0x146c8 - 0x40];
    UnkInfesterShot_l5c6b28 shot;  // 0x146c8
};

struct UnkInfesterOwner_l5c6b28 {
    u8 pad_0x0[0x38];
    u8 transform[0xc];  // 0x38
    u8 pad_0x44[0x10];
    f32 y;  // 0x54
    u8 pad_0x58[0x1f0 - 0x58];
    UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x48];
    UnkReflObject* ai;  // 0x240
    u8 pad_0x248[0x37c - 0x248];
    f32 radius;  // 0x37c
};

bool fn_71005cc54c(UnkInfesterAI_l5c6b28* ai, u32 side, UnkShotLimits_l5c6b28 limits, f32 reach);
bool fn_71005cbc14(UnkInfesterAI_l5c6b28* ai);
bool fn_71005cbc64(UnkInfesterAI_l5c6b28* ai);
bool fn_71005c6da0(CAttack* self);

static inline UnkInfesterOwner_l5c6b28* Owner(CAttack* self) {
    return reinterpret_cast<UnkInfesterOwner_l5c6b28*>(self->field_0x8);
}

// Whether `y` is below `top` raised by `radius`.
static inline bool Below(f32 top, f32 radius, f32 y) {
    return top + radius > y;
}

// The y of the owner's facing (the matrix's [5]).
static inline f32 Facing(CAttack* self) {
    return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(Owner(self)->transform)).m[5];
}

bool fn_71005c6b28(CAttack* self, void* arg, u32 mode) {
    bool ok = fn_7100360b84(self, arg, mode);
    UnkActor* target = fn_7100d7c990(self->field_0x8);
    if (!ok || !target)
        return false;
    UnkReflObject* aiObject = Owner(self)->ai;
    if (!aiObject || !fn_710009d50c(aiObject->vf_0x0(), fn_71005ca678()))
        return false;
    UnkInfesterAI_l5c6b28* ai = reinterpret_cast<UnkInfesterAI_l5c6b28*>(aiObject);
    UnkReflObject* movementObject = Owner(self)->movement;
    if (movementObject && !fn_710009d50c(movementObject->vf_0x0(), fn_71010c74e0()))
        movementObject = nullptr;
    if (fn_71010cc3a4(movementObject) != 1)
        return false;
    UnkInfesterMovement_l5c6b28* movement = reinterpret_cast<UnkInfesterMovement_l5c6b28*>(movementObject);

    UnkInfesterShot_l5c6b28& shot = ai->shot;
    bool inReach;
    if (shot.state->mode != 0 || !(Facing(self) > 0.0f) ||
        fn_71005cbcb4(reinterpret_cast<UnkInfesterAI_ovb0130*>(ai))) {
        inReach = true;
    } else {
        inReach = Below(Owner(self)->y, fn_7100d7c990(self->field_0x8)->field_0x37c,
                        fn_7100d7c990(self->field_0x8)->field_0x54) &
                  (ai->field_0x3f == 0);
    }
    f32 reach = Owner(self)->radius + 100.0f;
    UnkShotLimits_l5c6b28 limits = movement->limits;
    u32 side;
    fn_71005cc96c(reinterpret_cast<UnkInfesterSelf_l5cc96c*>(ai), &side);
    bool found = fn_71005cc54c(ai, static_cast<u8>(side), limits, reach);
    if (!inReach || !found || !shot.ready)
        return false;

    if (shot.state->mode == 0) {
        f32 facing = Facing(self);
        if ((facing + 0.01f >= 0.0f && facing - 0.01f <= 0.0f) || Facing(self) < 0.0f) {
            UnkActor* other = fn_7100d7c990(self->field_0x8);
            if (other->field_0x54 >= Owner(self)->y + Owner(self)->radius)
                return false;
        }
    }
    if (shot.state->mode != 0) {
        if (!fn_71005cbc14(ai))
            return false;
        if (shot.state->mode != 0)
            return !fn_71005c6da0(self);
    }
    if (!fn_71005cbc64(ai))
        return false;
    return !fn_71005c6da0(self);
}
