// CScorpiusPoisonousSpitMovementComponent's unit (tools/tu_map.py unit 1029): after the base movement update
// (fn_71010d1a90) the owner (+0x30) is turned to face along the velocity at +0x4b8 (its z angle, offset by pi)
// through its transform (+0x38) and its slot 0x118 (with mode 2), then its +0x3c0 is set. Names and layouts
// are placeholders.
#include <math.h>

#include "game/UnkMovementComponent.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkSpitOwner_ovb0124 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkXform transform;  // 0x38
    u8 pad_0x94[0x32c];
    bool field_0x3c0;

    void vf_0x118(UnkXform* xform, s32* mode) {
        reinterpret_cast<void (*)(UnkSpitOwner_ovb0124*, UnkXform*, s32*)>(vtable[0x118 / 8])(this, xform, mode);
    }
};

struct UnkSpitMovement_ovb0124 {
    u8 pad_0x0[0x30];
    UnkSpitOwner_ovb0124* owner;  // 0x30
    u8 pad_0x38[0x480];
    f32 vx, vy, vz;  // 0x4b8
};

void fn_7100699288(UnkSpitMovement_ovb0124* self, f32 dt) {
    fn_71010d1a90(reinterpret_cast<UnkMovementComponent*>(self), dt);
    f32 inv = 1.0f / (sqrtf(self->vx * self->vx + self->vy * self->vy + self->vz * self->vz) + 0.0001f);
    f32 x = self->vx * inv;
    f32 y = self->vy * inv;
    UnkSpitOwner_ovb0124* owner = self->owner;
    f32 angle = atan2f(y, x) + 3.1415927f;
    owner->transform.SetAngle(UnkVec3f(0.0f, 0.0f, angle));
    s32 mode = 2;
    owner->vf_0x118(&owner->transform, &mode);
    owner->field_0x3c0 = true;
}
