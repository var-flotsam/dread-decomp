// CInfesterBallAIComponent's unit (tools/tu_map.py, unit 898): stopping the roll, optionally turning round,
// then handing the mover and the owner's animation the new facing. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CEnemyMovement.h"
#include "game/behaviortree/CGoToSideAction.h"
#include "game/behaviortree/CSetAnimDependsOnDirAction.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::math::CVector2D;

// A direction with a flag saying whether it is set.
struct UnkOptDir_l5cfdc0 {
    f32 x, y, z;
    bool set;
};

// The mover at +0x146c8: its move direction (+0x40) and its last non-zero one (+0x70).
struct UnkInfesterMover_l5cfdc0 {
    void** vtable;
    u8 pad_0x8[0x38];
    f32 dir[3];  // 0x40
    u8 pad_0x4c[0x24];
    f32 last_dir[3];  // 0x70
};

// The block handed to the mover (fn_71010c7e6c).
struct UnkMoverParams_l5cfdc0 {
    void** vtable;
    f32 field_0x8;
    u8 field_0xc;
    u8 pad_0xd[7];
    u8 field_0x14;
    u8 pad_0x15[3];
    u8 field_0x18[13];
    u8 pad_0x25[3];
    u8 field_0x28[13];
    u8 pad_0x35[3];
    u32 field_0x38;
    u8 pad_0x3c;
    u8 field_0x3d;
    u8 field_0x3e;
    u8 field_0x3f;
    u8 pad_0x40;
    u8 field_0x41;
    u8 pad_0x42[2];
    u8 field_0x44[13];
    u8 field_0x51;
    u8 field_0x52;
};

struct UnkPair_l5cfdc0 {
    u64 first;
    u64 second;
};

// The kind-6 block handed to fn_71006f4b0c.
struct UnkAnimParams_l5cfdc0 {
    void** vtable;
    UnkOptDir_l5cfdc0 dir;
    f32 field_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    UnkPair_l5cfdc0 field_0x20;
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    u8 field_0x38;
    u8 field_0x39;
};

struct UnkInfesterMovement_l5cfdc0 {
    void** vtable;
    u8 pad_0x8[0xf1];
    bool active;  // 0xf9

    void vf_0x260() { reinterpret_cast<void (*)(UnkInfesterMovement_l5cfdc0*)>(vtable[0x260 / 8])(this); }
};

struct UnkInfesterOwner_l5cfdc0 {
    u8 pad_0x0[0x1f0];
    UnkInfesterMovement_l5cfdc0* movement;  // 0x1f0
    u8 pad_0x1f8[0x298 - 0x1f8];
    void* anim;  // 0x298
};

// The rolling state at +0x146c8.
struct UnkInfesterRoll_l5cfdc0 {
    UnkInfesterMover_l5cfdc0* mover;  // 0x0
    u8 pad_0x8[8];
    UnkPair_l5cfdc0 field_0x10;  // 0x10
    u8 pad_0x20[0x20];
    u32 state;  // 0x40
    u8 pad_0x44[1];
    bool backwards;  // 0x45
    u8 pad_0x46[3];
    bool field_0x49;
    u8 pad_0x4a[2];
    bool has_path;  // 0x4c
    u8 pad_0x4d[0x63];
    CVector2D direction;  // 0xb0
};

struct UnkInfesterBallAI_l5cfdc0 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkInfesterOwner_l5cfdc0* owner;  // 0x30
    u8 pad_0x38[0x146c8 - 0x38];
    UnkInfesterRoll_l5cfdc0 roll;  // 0x146c8
};
static_assert(__builtin_offsetof(UnkInfesterBallAI_l5cfdc0, roll) == 0x146c8, "layout");

static inline bool NearZero_l5cfdc0(f32 v) {
    return v + 0.001f >= 0.0f && v - 0.001f <= 0.0f;
}

void fn_71005cfdc0(UnkInfesterBallAI_l5cfdc0* self, bool turn) {
    UnkInfesterRoll_l5cfdc0& roll = self->roll;
    if (roll.state != 1)
        roll.state = 1;
    if (turn) {
        roll.backwards = !roll.backwards;
        f32 sign = roll.backwards ? 1.0f : -1.0f;
        roll.direction.x = sign;
        roll.direction.y = sign * 0.0f;
    }
    roll.field_0x49 = false;
    roll.has_path = false;

    UnkMoverParams_l5cfdc0 params;
    params.field_0x14 = 0;
    __builtin_memset(params.field_0x18, 0, 13);
    __builtin_memset(params.field_0x28, 0, 13);
    params.vtable = g_unk_7101a24db8 + 2;
    params.field_0x3d = 0;
    params.field_0x41 = 0;
    params.field_0x52 = 0;
    __builtin_memset(params.field_0x44, 0, 13);
    params.field_0x8 = 1.0f;
    params.field_0x38 = 1;
    params.field_0xc = 1;
    params.field_0x3e = 1;
    params.field_0x3f = 1;
    fn_71010c7e6c(reinterpret_cast<UnkEffectSink*>(roll.mover), &params, 1);

    UnkInfesterMover_l5cfdc0* mover = roll.mover;
    mover->dir[0] = roll.direction.x;
    mover->dir[1] = roll.direction.y;
    mover->dir[2] = 0.0f;
    f32 x = roll.direction.x;
    f32 y = roll.direction.y;
    mover = roll.mover;
    if (!(NearZero_l5cfdc0(x) && NearZero_l5cfdc0(y))) {
        mover->last_dir[0] = x;
        mover->last_dir[1] = y;
        mover->last_dir[2] = 0.0f;
    }

    reinterpret_cast<void (*)(UnkInfesterBallAI_l5cfdc0*)>(self->vtable[0x3d8 / 8])(self);
    UnkInfesterMovement_l5cfdc0* movement = self->owner->movement;
    if (movement->active) {
        movement->active = false;
        movement->vf_0x260();
    }

    UnkAnimParams_l5cfdc0 anim;
    anim.vtable = g_unk_7101a25fb0 + 2;
    anim.field_0x34 = 6;
    anim.field_0x20 = roll.field_0x10;
    anim.field_0x30 = 1;
    anim.field_0x38 = roll.backwards;
    anim.field_0x39 = 1;
    anim.field_0x18 = 1.0f;
    anim.field_0x1c = 1;
    anim.dir.x = roll.direction.x;
    anim.dir.y = roll.direction.y;
    anim.dir.z = 0.0f;
    anim.dir.set = true;
    fn_71006f4b0c(self->owner->anim, &anim, 1);
}
