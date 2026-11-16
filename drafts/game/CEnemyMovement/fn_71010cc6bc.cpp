// CEnemyMovement unit: adjusts a step's velocity before the owner's AI and its attack see it. In a
// CCharClassForceMoveInDesiredViewDirectionTrack the speed is turned into the desired direction; while
// melee limits apply (a crazy, push or allow-melee-limits track) and the enemy (fn_7100d7c990) moves us
// with its melee, the velocity is clamped by its push. Names and layouts are placeholders.
#include <math.h>

#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkVec3_cc6bc {
    f32 x, y, z;

    f32 Length() const { return sqrtf(x * x + y * y + z * z); }
};

// What one movement step carries.
struct UnkMoveStep_cc6bc {
    f32 dt;  // 0x0
    u8 pad_0x4[0x5c];
    UnkVec3_cc6bc velocity;  // 0x60
    UnkVec3_cc6bc applied;   // 0x6c
};

struct UnkMoveAI_cc6bc {
    void vf_0x5f0(UnkMoveStep_cc6bc* step) {
        reinterpret_cast<void (*)(UnkMoveAI_cc6bc*, UnkMoveStep_cc6bc*)>(
            (*reinterpret_cast<void***>(this))[0x5f0 / 8])(this, step);
    }
};
struct UnkMoveAttack_cc6bc {
    void vf_0xc0(UnkMoveStep_cc6bc* step) {
        reinterpret_cast<void (*)(UnkMoveAttack_cc6bc*, UnkMoveStep_cc6bc*)>(
            (*reinterpret_cast<void***>(this))[0xc0 / 8])(this, step);
    }
};
struct UnkMoveAttackHolder_cc6bc {
    u8 pad_0x0[0x1a0];
    UnkMoveAttack_cc6bc* attack;  // 0x1a0
};

struct UnkMoveOwner_cc6bc {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkMoveAI_cc6bc* ai;  // 0x240
    u8 pad_0x248[0x30];
    UnkMoveAttackHolder_cc6bc* attack;  // 0x278
};

// The enemy's melee: its push per second and its strength.
struct UnkMelee_cc6bc {
    u8 pad_0x0[0x10c];
    f32 push;  // 0x10c
    u8 pad_0x110[0x2ce8 - 0x110];
    f32 strength;  // 0x2ce8
};

struct UnkEnemyMovement_cc6bc {
    u8 pad_0x0[0x30];
    UnkMoveOwner_cc6bc* owner;  // 0x30
    u8 pad_0x38[0x38];
    UnkVec3_cc6bc desired_dir;  // 0x70
    u8 pad_0x7c[0x1be1 - 0x7c];
    bool melee_limits;  // 0x1be1

    bool InTrack(const base::reflection::CClass* (*cls)()) {
        UnkAnimationComponent* animation = owner->animation;
        return animation && fn_710117b348(animation, cls());
    }
};

void fn_71010cc6bc(UnkEnemyMovement_cc6bc* self, UnkMoveStep_cc6bc* step) {
    if (self->InTrack(fn_7100c1e968)) {
        f32 speed = step->velocity.Length();
        f32 inv = 1.0f / (self->desired_dir.Length() + 0.0001f);
        UnkVec3_cc6bc dir = {self->desired_dir.x * inv, self->desired_dir.y * inv, self->desired_dir.z * inv};
        UnkVec3_cc6bc v = {speed * dir.x, speed * dir.y, speed * dir.z};
        step->velocity = v;
        step->applied = v;
    }
    if (self->melee_limits &&
        (self->InTrack(fn_7100cf3508) || self->InTrack(fn_7100d253f8) || self->InTrack(fn_7100bf2cdc))) {
        UnkActor* enemy = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
        if (enemy && enemy->field_0x208 && enemy->field_0x220 &&
            fn_710117b348(static_cast<UnkAnimationComponent*>(enemy->field_0x220), fn_7100cec578())) {
            UnkMelee_cc6bc* melee = static_cast<UnkMelee_cc6bc*>(enemy->field_0x208);
            f32 limit = melee->strength * step->dt;
            if (!(limit + 0.001f >= 0.0f) || !(limit - 0.001f <= 0.0f)) {
                f32 push = step->dt * melee->push;
                f32 x = push + step->velocity.x;
                if (limit > 0.0f) {
                    step->velocity.x = x > limit ? x : limit;
                    f32 y = push + step->applied.x;
                    step->applied.x = y > limit ? y : limit;
                } else {
                    step->velocity.x = x < limit ? x : limit;
                    f32 y = push + step->applied.x;
                    step->applied.x = y < limit ? y : limit;
                }
            }
        }
    }
    UnkMoveOwner_cc6bc* owner = self->owner;
    if (owner->ai)
        owner->ai->vf_0x5f0(step);
    UnkMoveAttackHolder_cc6bc* holder = self->owner->attack;
    if (holder && holder->attack)
        holder->attack->vf_0xc0(step);
}
