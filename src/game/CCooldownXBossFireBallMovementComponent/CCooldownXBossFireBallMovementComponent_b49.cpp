// CCooldownXBossFireBallMovementComponent (tools/tu_map.py): after the base movement update
// (fn_7100ac3a78, src/game/CNailongThornMovementComponent/CNailongThornMovementComponent.cpp) turns the
// owner (+0x30) to the angles (0, -pi/2, 0) through its transform (+0x38, game/UnkXformNode.h) and its
// slot 0x118 (with mode 2), then sets its +0x3c0. Names and layouts are placeholders.
#include "game/CProjectileMovement.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkNailongThornMovement;

struct UnkFireBallOwner_b49 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkXform transform;
    u8 pad_0x94[0x32c];
    bool field_0x3c0;

    void vf_0x118(UnkXform* xform, s32* mode) {
        reinterpret_cast<void (*)(UnkFireBallOwner_b49*, UnkXform*, s32*)>(vtable[0x118 / 8])(this, xform, mode);
    }
};

struct UnkFireBallMovement_b49 {
    u8 pad_0x0[0x30];
    UnkFireBallOwner_b49* owner;
};

void fn_71004d75c8(UnkFireBallMovement_b49* self, f32 dt) {
    fn_7100ac3a78(reinterpret_cast<UnkNailongThornMovement*>(self), dt);
    UnkFireBallOwner_b49* owner = self->owner;
    owner->transform.SetAngle(UnkVec3f(0.0f, -1.5707964f, 0.0f));
    s32 mode = 2;
    owner->vf_0x118(&owner->transform, &mode);
    owner->field_0x3c0 = true;
}
