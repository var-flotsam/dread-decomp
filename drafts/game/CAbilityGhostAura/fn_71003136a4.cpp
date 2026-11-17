// CAbilityGhostAura's unit (tools/tu_map.py, unit 430): the dash step on the owner's motion. While the dash runs
// it advances the dash progress (+0x460) along the probed path from the player, keeping the step taken (+0x414)
// and the distance covered and left (+0x468, +0x46c), and then writes the step, or a stop, into the motion's two
// vectors. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector3D;

// As src/game/CAbilityGhostAura/CAbilityGhostAura_Update_ovb0040.cpp declares them.
struct UnkGhostAura_b0040;
struct UnkGhostAuraPos_b0040;
void fn_7100314288(UnkGhostAura_b0040* self);
void fn_710031410c(UnkGhostAura_b0040* self);
bool fn_71003139c0(UnkGhostAura_b0040* self, UnkGhostAuraPos_b0040* pos, void* a2, void* a3, f32 value);

struct UnkGhostAuraMovement_l3136a4 {
    void** vtable;

    bool vf_0x178() { return reinterpret_cast<bool (*)(UnkGhostAuraMovement_l3136a4*)>(vtable[0x178 / 8])(this); }
};

struct UnkGhostAuraActor_l3136a4 {
    u8 pad_0x0[0x208];
    UnkGhostAuraMovement_l3136a4* movement;  // 0x208
};

struct UnkGhostAuraLink_l3136a4 {
    u8 pad_0x0[0x30];
    UnkGhostAuraActor_l3136a4* actor;  // 0x30
};

struct UnkGhostAuraPlayer_l3136a4 {
    u8 pad_0x0[0x38];
    f32 transform[3][4];  // 0x38

    CVector3D Position() const { return {transform[0][3], transform[1][3], transform[2][3]}; }
};

struct UnkGhostAuraMotion_l3136a4 {
    u8 pad_0x0[0x60];
    UnkVec3f velocity;  // 0x60
    UnkVec3f field_0x6c;

    void SetVelocity(const UnkVec3f& v) {
        velocity = v;
        field_0x6c = velocity;
    }
};

struct UnkGhostAura_l3136a4 {
    u8 pad_0x0[0xc0];
    UnkGhostAuraLink_l3136a4* link;  // 0xc0
    u8 pad_0xc8[0x308];
    u32 field_0x3d0;
    u8 pad_0x3d4[0x40];
    UnkVec3f step;  // 0x414
    u8 pad_0x420[0x28];
    s32 state;  // 0x448
    s32 field_0x44c;
    u8 pad_0x450[0x10];
    f32 progress;  // 0x460
    u8 pad_0x464[4];
    f32 covered;    // 0x468
    f32 remaining;  // 0x46c
    f32 duration;   // 0x470
    f32 rate;       // 0x474
    u8 pad_0x478[4];
    f32 charge;  // 0x47c
    u8 pad_0x480[0x18];
    bool field_0x498;
};
static_assert(__builtin_offsetof(UnkGhostAura_l3136a4, field_0x498) == 0x498, "layout");

bool fn_7100309204(UnkGhostAura_l3136a4* self);

static inline UnkGhostAura_b0040* Base(UnkGhostAura_l3136a4* self) {
    return reinterpret_cast<UnkGhostAura_b0040*>(self);
}

static inline bool Probe(UnkGhostAura_l3136a4* self, UnkVec3f* pos, void* a2, void* a3) {
    return fn_71003139c0(Base(self), reinterpret_cast<UnkGhostAuraPos_b0040*>(pos), a2, a3, self->progress);
}

// The shared zero vector, as the motion's vector type.
static inline const UnkVec3f& Zero() { return reinterpret_cast<const UnkVec3f&>(g_unk_7101647790); }

static inline bool IsNearlyZero(f32 v) { return (v - 1e-5f <= 0.0f) & (v + 1e-5f >= 0.0f); }

void fn_71003136a4(UnkGhostAura_l3136a4* self, UnkGhostAuraMotion_l3136a4* motion) {
    if (self->link->actor->movement->vf_0x178())
        return;
    if (self->state == 0)
        return;
    if (self->charge > 0.0f && fn_7100309204(self)) {
        motion->SetVelocity(UnkVec3f(0.0f, 0.0f, 0.0f));
        return;
    }
    self->step = motion->velocity;
    f32 length_sq = self->step.x * self->step.x + self->step.y * self->step.y + self->step.z * self->step.z;
    f32 rate = self->rate;
    if (length_sq != 0.0f && self->duration > 0.1f)
        rate = sqrtf(length_sq) / self->duration;

    if (self->progress < 1.0f && self->field_0x3d0 >= 2) {
        auto* player = reinterpret_cast<UnkGhostAuraPlayer_l3136a4*>(fn_7100dec788(g_unk_7101d47310));
        if (player) {
            CVector3D from = player->Position();
            UnkVec3f start(0.0f, 0.0f, 0.0f);
            if (Probe(self, &start, nullptr, nullptr)) {
                f32 progress = rate + self->progress;
                self->progress = !(progress >= 1.0f) ? progress : 1.0f;
                u8 hit = 0;
                UnkVec3f normal(0.0f, 0.0f, 0.0f);
                UnkVec3f end(0.0f, 0.0f, 0.0f);
                if (Probe(self, &end, &hit, &normal)) {
                    self->step.y = end.y - from.y;
                    self->step.x = end.x - from.x;
                    self->step.z = end.z - from.z;
                    f32 dx = end.x - start.x;
                    f32 dy = end.y - start.y;
                    f32 dz = end.z - start.z;
                    f32 distance = sqrtf(dx * dx + dy * dy + dz * dz);
                    if (!IsNearlyZero(end.x * end.x + end.y * end.y + end.z * end.z) && distance < 800.0f) {
                        self->covered = distance + self->covered;
                        self->remaining = self->remaining - distance;
                        fn_710031410c(Base(self));
                    }
                }
            }
        }
    } else if ((self->state & ~1) == 4) {
        fn_710031410c(Base(self));
        self->field_0x44c = 6;
    }

    s32 state = self->state;
    if (static_cast<u32>(state - 3) <= 2) {
        if (state != 5 || !self->field_0x498) {
            motion->SetVelocity(self->step);
        }
    } else if (state == 6) {
        if (self->remaining <= 0.0f) {
            motion->SetVelocity(Zero());
        } else {
            fn_7100314288(Base(self));
        }
    }
    fn_710031410c(Base(self));
}
