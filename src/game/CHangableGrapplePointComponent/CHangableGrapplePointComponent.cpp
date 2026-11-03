// CHangableGrapplePointComponent: fn_7100824a58 gives the object at +0x228 of the actor (+0x30),
// when there is one, the negated angles of the actor's transform (+0x10c). Names and layouts are
// placeholders.
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkHangableGrappleTarget {
    u8 pad_0x0[0x10c];
    UnkVec3f field_0x10c;
};

struct UnkHangableGrapplePoint {
    u8 pad_0x0[0x30];
    UnkActor* actor;
};

void fn_7100824a58(UnkHangableGrapplePoint* self) {
    UnkActor* actor = self->actor;
    if (UnkHangableGrappleTarget* target = reinterpret_cast<UnkHangableGrappleTarget*>(actor->field_0x228)) {
        UnkVec3f angle = reinterpret_cast<UnkXform*>(actor->field_0x38)->GetAngle();
        target->field_0x10c = UnkVec3f(-angle.x, -angle.y, -angle.z);
    }
}
