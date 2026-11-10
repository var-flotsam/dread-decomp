// CCharClassRotateToViewDirEvent's unit (tools/tu_map.py unit 2275): fn_7100ccf664 runs the event on
// an actor: it turns the actor to face its view direction (the yaw of the direction at its +0x1f0,
// +0x58/+0x60, made into a facing by fn_710007a220). When the actor has an active (+0x396) rotation
// component (+0x208), the component's transform (+0x338) gets the angles and the component is told
// whether to apply them at once (+0x395, from the event's +0xc); otherwise an immediate event turns
// the actor through its vtable slot 0xd8, and a smooth one sets its own transform (+0x38) and marks
// it (+0x3c0). Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

extern "C" float atan2f(float y, float x);
UnkVec3f fn_710007a220(const UnkVec3f* v);

struct UnkViewDir_ovb0134 {
    u8 pad_0x0[0x58];
    f32 x;  // 0x58
    u8 pad_0x5c[0x4];
    f32 z;  // 0x60
};

struct UnkRotateComponent_ovb0134 {
    u8 pad_0x0[0x338];
    UnkXform xform;  // 0x338
    u8 pad_0x394[0x1];
    bool immediate;  // 0x395
    bool active;     // 0x396
};

struct UnkRotateActor_ovb0134 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8(const UnkVec3f* angle, bool now);  // turns the actor

    u8 pad_0x8[0x30];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x15c];
    UnkViewDir_ovb0134* view_dir;  // 0x1f0
    u8 pad_0x1f8[0x10];
    UnkRotateComponent_ovb0134* rotation;  // 0x208
    u8 pad_0x210[0x1b0];
    bool turned;  // 0x3c0
};
static_assert(__builtin_offsetof(UnkRotateActor_ovb0134, turned) == 0x3c0, "layout");

struct UnkActorRef_ovb0134 {
    UnkRotateActor_ovb0134* actor;
};

struct UnkRotateEvent_ovb0134 {
    u8 pad_0x0[0xc];
    bool immediate;  // 0xc
};

// The facing for a yaw angle.
static inline UnkVec3f FacingForYaw(f32 yaw) {
    UnkVec3f angles(0.0f, yaw, 0.0f);
    return fn_710007a220(&angles);
}

void fn_7100ccf664(UnkRotateEvent_ovb0134* self, UnkActorRef_ovb0134* ref) {
    if (!ref->actor)
        return;
    UnkViewDir_ovb0134* dir = ref->actor->view_dir;
    if (!dir)
        return;
    bool immediate = self->immediate;
    UnkVec3f facing = FacingForYaw(atan2f(dir->x, dir->z));
    UnkRotateActor_ovb0134* actor = ref->actor;
    UnkRotateComponent_ovb0134* rotation = actor->rotation;
    if (rotation && rotation->active) {
        rotation->immediate |= immediate;
        rotation->xform.SetAngle(facing);
    } else if (immediate) {
        actor->vf_0xd8(&facing, true);
    } else {
        actor->xform.SetAngle(facing);
        actor->turned = true;
    }
}
