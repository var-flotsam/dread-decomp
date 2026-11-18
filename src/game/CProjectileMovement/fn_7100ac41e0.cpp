// CProjectileMovement unit (tools/tu_map.py): one movement step over `dt`. Applies gravity
// (vf_0x2d0) to the velocity at +0x4b8 when falling (vf_0x180) and the definition allows it, adds the
// acceleration vf_0x360 gives, then moves `pos` by the velocity, the drift at +0x238 and, while it
// lasts, the push at +0x4f8 scaled by the decaying strength at +0x508. Names and layouts are placeholders.
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkHomingMovement;

struct UnkProjectileDef_lac41e0 {
    u8 pad_0x0[0x130];
    bool no_gravity;  // 0x130
};

struct UnkProjectileMovement_lac41e0;

struct UnkProjectileMovementVtbl_lac41e0 {
    void* slots[0x180 / 8];
    bool (*vf_0x180)(UnkProjectileMovement_lac41e0* self);
    void* slots_0x188[(0x2d0 - 0x188) / 8];
    f32 (*vf_0x2d0)(UnkProjectileMovement_lac41e0* self);
    void* slots_0x2d8[(0x360 - 0x2d8) / 8];
    UnkVec3f (*vf_0x360)(UnkProjectileMovement_lac41e0* self, f32 dt);
};

struct UnkProjectileMovement_lac41e0 {
    UnkProjectileMovementVtbl_lac41e0* vtbl;
    u8 pad_0x8[0x238 - 0x8];
    UnkVec3f drift;  // 0x238
    u8 pad_0x244[0x4b8 - 0x244];
    UnkVec3f velocity;  // 0x4b8
    u8 pad_0x4c4[0x4f8 - 0x4c4];
    UnkVec3f push;        // 0x4f8
    f32 push_strength;    // 0x504
    f32 push_level;       // 0x508
    f32 push_decay;       // 0x50c
    u8 pad_0x510[0x5c8 - 0x510];
    UnkProjectileDef_lac41e0* def;  // 0x5c8
};
static_assert(__builtin_offsetof(UnkProjectileMovement_lac41e0, push_level) == 0x508, "layout");
static_assert(__builtin_offsetof(UnkProjectileMovement_lac41e0, def) == 0x5c8, "layout");

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_7100ac41e0(UnkHomingMovement* movement, void* ctx, f32 dt) {
    auto* self = reinterpret_cast<UnkProjectileMovement_lac41e0*>(movement);
    UnkVec3f* pos = static_cast<UnkVec3f*>(ctx);
    if (self->vtbl->vf_0x180(self) && self->def && !self->def->no_gravity)
        self->velocity.y -= self->vtbl->vf_0x2d0(self) * dt;
    UnkVec3f accel = self->vtbl->vf_0x360(self, dt);
    self->velocity.x = accel.x + self->velocity.x;
    self->velocity.y = accel.y + self->velocity.y;
    self->velocity.z = accel.z + self->velocity.z;
    UnkVec3f next = self->velocity * dt + *pos;
    *pos = next;
    next = next + self->drift * dt;
    *pos = next;
    f32 level = self->push_level;
    // Only while the push is not (nearly) zero.
    if (!(level + 0.00001f >= 0.0f && level - 0.00001f <= 0.0f)) {
        *pos = next + self->push * (level * dt);
        if (self->push_decay > 0.0f) {
            f32 t = self->push_decay * dt;
            self->push_level = self->push_level - (!(t >= 1.0f) ? t : 1.0f) * self->push_level;
        }
    }
}
