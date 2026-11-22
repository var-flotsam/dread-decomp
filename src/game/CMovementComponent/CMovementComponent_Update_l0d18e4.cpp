// CMovementComponent unit (tools/tu_map.py unit 2688): the update. It keeps a copy of the owner's
// transform block (+0x38 of the owner), runs the base update and slot 0x1d0, then moves the position
// (+0x1d8) by the velocity (+0x208) over the step (slowed for the player in slow motion) and clears
// the velocity and the vector at +0x1e4. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkSlomoFlags.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2689.h"
#include "types.h"

using base::math::CVector3D;

struct UnkTransformBlock_l0d18e4 {
    f32 values[23];

    UnkTransformBlock_l0d18e4& operator=(const UnkTransformBlock_l0d18e4& other) {
        if (&other != this) {
            for (int i = 0; i < 23; i++)
                values[i] = other.values[i];
        }
        return *this;
    }
};

struct UnkMovementOwner_l0d18e4 {
    u8 pad_0x0[0x38];
    UnkTransformBlock_l0d18e4 transform;  // 0x38
};

struct UnkMovement_l0d18e4;

struct UnkMovementVtable_l0d18e4 {
    u8 pad_0x0[0x1d0];
    void (*UpdateMovement)(UnkMovement_l0d18e4* self, f32 dt);
};

struct UnkMovement_l0d18e4 {
    UnkMovementVtable_l0d18e4* vtable;
    u8 pad_0x8[0x28];
    UnkMovementOwner_l0d18e4* owner;  // 0x30
    u8 pad_0x38[0x64];
    UnkTransformBlock_l0d18e4 last_transform;  // 0x9c
    u8 pad_0xf8[0xe0];
    CVector3D position;  // 0x1d8
    CVector3D field_0x1e4;
    u8 pad_0x1f0[0x18];
    CVector3D velocity;  // 0x208
};
static_assert(__builtin_offsetof(UnkMovement_l0d18e4, position) == 0x1d8, "layout");
static_assert(__builtin_offsetof(UnkMovement_l0d18e4, field_0x1e4) == 0x1e4, "layout");
static_assert(__builtin_offsetof(UnkMovement_l0d18e4, velocity) == 0x208, "layout");

void fn_71010d18e4(UnkMovement_l0d18e4* self, f32 dt) {
    self->last_transform = self->owner->transform;
    // The base update takes the step too; include/game/CComponent.h declares it without one.
    using BaseUpdate = void (*)(char*, f32);
    reinterpret_cast<BaseUpdate>(fn_71010c4234)(reinterpret_cast<char*>(self), dt);
    self->vtable->UpdateMovement(self, dt);
    UnkGameManager* manager = g_unk_7101d47310;
    if (manager->field_0xc8 != 0 || manager->field_0x10c) {
        UnkSlomoFlags* slomo = reinterpret_cast<UnkSlomoFlags*>(&manager->field_0xc4);
        if (slomo->enabled && !slomo->paused &&
            reinterpret_cast<UnkPlayer*>(self->owner) == fn_7100dec788(manager))
            dt = fn_71010d6858() * dt;
    }
    f32 dx = dt * self->velocity.x;
    f32 dy = dt * self->velocity.y;
    f32 dz = dt * self->velocity.z;
    self->velocity.x = 0.0f;
    self->velocity.y = 0.0f;
    self->velocity.z = 0.0f;
    self->position.x = dx + self->position.x;
    self->position.y = dy + self->position.y;
    self->position.z = dz + self->position.z;
    self->field_0x1e4.x = 0.0f;
    self->field_0x1e4.y = 0.0f;
    self->field_0x1e4.z = 0.0f;
}
