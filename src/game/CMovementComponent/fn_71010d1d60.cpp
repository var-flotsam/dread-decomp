// CMovementComponent unit (tools/tu_map.py unit 2688): moves the owner to a position. Unless the owner
// plays an ignore-gravity or flying track, the position is first dropped onto the ground under it
// (fn_71010d15d0, lifted by the shape's radius). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/CMovementComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::math::CVector3D;

struct UnkMovementShape_l0d1d60 {
    u8 pad_0x0[0x60];
    f32 radius;
};

struct UnkMovementOwner_l0d1d60;

struct UnkMovementOwnerVtable_l0d1d60 {
    u8 pad_0x0[0xd0];
    void (*SetPosition)(UnkMovementOwner_l0d1d60* self, const CVector3D* pos, bool force);
};

struct UnkMovementOwner_l0d1d60 {
    UnkMovementOwnerVtable_l0d1d60* vtable;
    u8 pad_0x8[0x218];
    UnkAnimationComponent* animation;
    UnkMovementShape_l0d1d60* shape;
};

struct UnkMovement_l0d1d60;

struct UnkMovementVtable_l0d1d60 {
    u8 pad_0x0[0x190];
    void (*OnMoveTo)(UnkMovement_l0d1d60* self, const CVector3D* pos);
};

struct UnkMovement_l0d1d60 {
    UnkMovementVtable_l0d1d60* vtable;
    u8 pad_0x8[0x28];
    UnkMovementOwner_l0d1d60* owner;
};

// Whether the owner's animation component plays a track of the class the getter returns.
inline bool PlaysTrack(UnkMovementOwner_l0d1d60* owner, const base::reflection::CClass* (*getClass)()) {
    UnkAnimationComponent* animation = owner->animation;
    return animation && fn_710117b348(animation, getClass());
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71010d1d60(UnkMovement_l0d1d60* self, const CVector3D* pos) {
    CVector3D target;
    target.x = pos->x;
    target.y = pos->y;
    target.z = pos->z;
    CVector3D ground;
    ground.x = 0.0f;
    ground.y = 0.0f;
    ground.z = 0.0f;
    if (!PlaysTrack(self->owner, fn_7100d0c7a8) && !PlaysTrack(self->owner, fn_7100cf96a8)) {
        if (fn_71010d15d0(reinterpret_cast<char*>(self), pos,
                          reinterpret_cast<UnkProbeResult*>(&ground), self->owner->shape->radius)) {
            target.x = ground.x;
            target.z = ground.z;
            target.y = ground.y + self->owner->shape->radius;
        }
    }
    UnkAnimationComponent* animation;
    animation = self->owner->animation;
    if (!(animation && fn_710117aaac(animation, fn_7100d11078(), 0.0f)))
        self->vtable->OnMoveTo(self, &target);
    UnkMovementOwner_l0d1d60* owner = self->owner;
    owner->vtable->SetPosition(owner, &target, true);
}
