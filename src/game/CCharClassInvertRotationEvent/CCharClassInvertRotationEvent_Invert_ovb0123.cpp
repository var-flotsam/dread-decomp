// CCharClassInvertRotationEvent's unit (tools/tu_map.py): the event negates the angles of the
// target actor's transform (+0x38, game/UnkXformNode.h), notifies the actor through its slot 0x118
// (mode 2) when the event's flag (+0xc) is set, and marks the actor's +0x3c0. Names and layouts are
// placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkInvertRotationActor_ovb0123 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkXform transform;  // 0x38
    u8 pad_0x94[0x32c];
    bool field_0x3c0;

    void vf_0x118(UnkXform* xform, s32* mode) {
        reinterpret_cast<void (*)(UnkInvertRotationActor_ovb0123*, UnkXform*, s32*)>(vtable[0x118 / 8])(this, xform,
                                                                                                    mode);
    }
};
static_assert(__builtin_offsetof(UnkInvertRotationActor_ovb0123, field_0x3c0) == 0x3c0, "layout");

struct UnkInvertRotationEvent_ovb0123 {
    u8 pad_0x0[0xc];
    bool notify;  // 0xc
};

static inline UnkVec3f Inverted(const UnkVec3f& v) {
    return UnkVec3f(-v.x, -v.y, -v.z);
}

void fn_7100cc52e4(UnkInvertRotationEvent_ovb0123* self, UnkInvertRotationActor_ovb0123** target) {
    UnkInvertRotationActor_ovb0123* actor = *target;
    if (!actor)
        return;
    // The transform reached as the end of the bytes before it: the original computes its address
    // separately for the read and for the update.
    UnkVec3f angle = reinterpret_cast<UnkXform*>(&actor->pad_0x8[0x30])->GetAngle();
    UnkVec3f inverted = Inverted(angle);
    bool notify = self->notify;
    UnkXform* xform = &actor->transform;
    xform->SetAngle(inverted);
    if (notify) {
        s32 mode = 2;
        actor->vf_0x118(xform, &mode);
    }
    actor->field_0x3c0 = true;
}
