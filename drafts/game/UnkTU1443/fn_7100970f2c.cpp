// Unit 1443: one movement step. When the movement starts its action, the step casts from the position
// along the velocity and stops when blocked; otherwise, unless the owner is held, it plays the action
// and applies the motion. The velocity is then clamped to the maximum speed. Names and layouts are
// placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/math/CVector.h"
#include "base/math/CVectorOps.h"
#include "game/CAnimationComponent.h"
#include "game/CFrozenComponent.h"
#include "game/UnkTU1730.h"
#include "types.h"

#include <float.h>

using base::math::CVector3D;

// The action request handed to the movement and to the animation player.
struct UnkActionRequest_970f2c {
    s64 id;
    u64 desc[2];     // 0x8
    u64 blend_kind;  // 0x18
    u32 blend_value; // 0x20
    u32 flags;       // 0x24
    u32 mask;        // 0x28
};

struct UnkActionDesc_970f2c {
    u64 words[2];
};

struct UnkActionBlend_970f2c {
    u64 kind;
    u32 value;
};

// The movement: an action state at +0x6f8 and a cast between two points (vtable slot 0x390).
struct UnkMovement_970f2c {
    void** vtable;
    u8 pad_0x8[0x6f0];
    u32 state;  // 0x6f8

    // Casts from `from` to `to`; `hit` gets where it stopped. True when nothing was in the way.
    bool Cast(const CVector3D* from, const CVector3D* to, CVector3D* hit) {
        typedef bool (*Fn)(UnkMovement_970f2c*, const CVector3D*, const CVector3D*, CVector3D*);
        return reinterpret_cast<Fn>(vtable[0x390 / 8])(this, from, to, hit);
    }
};
static_assert(__builtin_offsetof(UnkMovement_970f2c, state) == 0x6f8, "UnkMovement_970f2c layout");

struct UnkHold_970f2c {
    u8 pad_0x0[0x70];
    u32 kind;  // 0x70
};

struct UnkMoveOwner_970f2c {
    u8 pad_0x0[0x1b8];
    UnkHold_970f2c* hold;  // 0x1b8
    u8 pad_0x1c0[0x60];
    UnkStruct_7101176ea8* animation;  // 0x220
    u8 pad_0x228[0xa0];
    void* frozen;  // 0x2c8
};
static_assert(__builtin_offsetof(UnkMoveOwner_970f2c, frozen) == 0x2c8, "UnkMoveOwner_970f2c layout");

struct UnkMover_970f2c {
    u8 pad_0x0[0x30];
    UnkMoveOwner_970f2c* owner;     // 0x30
    UnkMovement_970f2c* movement;  // 0x38
    u8 pad_0x40[0x60];
    f32 max_speed;     // 0xa0
    bool clamp_speed;  // 0xa4
};

// The step's state: the time, a transform (3x4, its translation in the last column), the motion and
// the velocity.
struct UnkMoveState_970f2c {
    f32 time;
    f32 transform[3][4];  // 0x4
    u8 pad_0x34[0x2c];
    CVector3D motion;    // 0x60
    CVector3D velocity;  // 0x6c
    u8 block_0x78[0x40];
    u8 block_0xb8[0x8];

    CVector3D Translation() const { return CVector3D{transform[0][3], transform[1][3], transform[2][3]}; }
};
static_assert(__builtin_offsetof(UnkMoveState_970f2c, block_0xb8) == 0xb8, "UnkMoveState_970f2c layout");

struct UnkMovement3_ovb0048;
void fn_71010d1e88(UnkMovement3_ovb0048* self, void* a, void* b, f32 t, void* c, void* d, void* e);
void fn_71010c98ec(UnkMovement_970f2c* movement, u32* state, UnkActionRequest_970f2c* request);

// The key of the empty id.
static constexpr u64 kEmptyKey = 0xf4061ff25a2163d6ull;

// Whether two points are within `tolerance` of each other on every axis.
inline bool IsNear(const CVector3D& a, const CVector3D& b, f32 tolerance) {
    return a.x + tolerance >= b.x && a.x - tolerance <= b.x && a.y + tolerance >= b.y &&
           a.y - tolerance <= b.y && a.z + tolerance >= b.z && a.z - tolerance <= b.z;
}

void fn_7100970f2c(UnkMover_970f2c* self, UnkMoveState_970f2c* state, void* out) {
    if (self->owner->animation) {
        UnkActionRequest_970f2c request;
        const UnkActionDesc_970f2c* desc = reinterpret_cast<const UnkActionDesc_970f2c*>(&g_unk_710164b958);
        u64 desc0 = desc->words[0];
        u64 desc1 = desc->words[1];
        const UnkActionBlend_970f2c* blend = reinterpret_cast<const UnkActionBlend_970f2c*>(&g_unk_7172769740);
        u64 kind = blend->kind;
        u32 value = blend->value;
        u32 mask = UnkAll_SpitSend().value;
        request.id = -1;
        request.desc[0] = desc0;
        request.desc[1] = desc1;
        request.blend_kind = kind;
        request.blend_value = value;
        request.flags = 0;
        request.mask = mask;
        UnkMovement_970f2c* movement = self->movement;
        fn_71010c98ec(movement, &movement->state, &request);
        if (request.id != -1) {
            CVector3D from = state->Translation();
            CVector3D to = from;
            to = state->velocity + to;
            CVector3D hit = to;
            const CVector3D zero = {0.0f, 0.0f, 0.0f};
            if (!IsNear(state->velocity, zero, 0.1f) && !self->movement->Cast(&from, &to, &hit) &&
                IsNear(from, hit, 0.1f)) {
                // Blocked where it stands.
                self->movement->state = 6;
                state->velocity = zero;
                state->motion = zero;
            } else {
                UnkMoveOwner_970f2c* owner = self->owner;
                bool held = owner->hold && owner->hold->kind == 2;
                bool frozen = owner->frozen && fn_71007f9f20(owner->frozen);
                if (!held && !frozen &&
                    fn_7101176ea8(self->owner->animation,
                                  reinterpret_cast<const UnkRequest_7101176ea8*>(&request), kEmptyKey)) {
                    state->motion = zero;
                    state->velocity = zero;
                    fn_71010d1e88(reinterpret_cast<UnkMovement3_ovb0048*>(self->movement), state->block_0xb8,
                                  &state->motion, state->time, &state->velocity, state->block_0x78, out);
                }
            }
        }
    }

    CVector3D velocity = state->velocity;
    f32 length = base::math::Length(velocity);
    if (length > 0.0f && self->clamp_speed && self->max_speed < FLT_MAX && length > self->max_speed) {
        CVector3D clamped = velocity * self->max_speed * (1.0f / length);
        state->motion = clamped;
        state->velocity = clamped;
    }
}
