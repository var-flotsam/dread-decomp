// Unit 1277 (0x7100816e70-0x7100821f40), whose reflected class is CGrappleBeamComponent. Holds
// fn_710081b434, a test the grapple code makes before it goes on (the Flash Shift ability's unit
// calls it on the grapple beam gun's component). Names and layouts are placeholders inferred from
// the code.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkXformNode.h"
#include "types.h"

// A component: its actor at +0x10.
struct UnkGrappleOwnerComponent {
    u8 unk_0x0[0x10];
    UnkActor* actor;
};

// What the grapple is attached to: a value at +0x118 and a word passed on at +0x3c8.
struct UnkGrappleTarget {
    u8 unk_0x0[0x118];
    f32 field_0x118;
    u8 unk_0x11c[0x2ac];
    u32 field_0x3c8;
};

struct UnkGrappleTargetHolder {
    u8 unk_0x0[0x208];
    UnkGrappleTarget* target;
};

struct UnkGrappleLink {
    u8 unk_0x0[0x18];
    UnkGrappleTargetHolder* holder;
};

struct UnkGrappleComponent {
    u8 unk_0x0[0x230];
    UnkGrappleOwnerComponent* owner;
    u8 unk_0x238[0x2f8];
    UnkGrappleLink* link;
};

bool fn_7100976c94(UnkGrappleTarget* target, const UnkVec3f* pos, u32 value, void* arg, bool flag, f32 offset);

static inline bool IsNear(f32 a, f32 b) {
    return a - 0.001f <= b && a + 0.001f >= b;
}

// False while the actor plays a track of class CCharClassIgnoreGrappleAbortTrack; else true when the
// target's value at +0x118 is zero, else whether fn_7100976c94 fails for the actor's position.
bool fn_710081b434(UnkGrappleComponent* self) {
    if (void* anim = self->owner->actor->field_0x220) {
        if (fn_710117b348(static_cast<UnkAnimationComponent*>(anim), fn_7100d0bd88()))
            return false;
    }
    UnkGrappleTarget* target = self->link->holder->target;
    if (IsNear(target->field_0x118, 0.0f))
        return true;
    UnkVec3f pos = reinterpret_cast<const UnkXform*>(self->owner->actor->field_0x38)->GetTranslation();
    return !fn_7100976c94(target, &pos, target->field_0x3c8, nullptr, true, -0.1f);
}
