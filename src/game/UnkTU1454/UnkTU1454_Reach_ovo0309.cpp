// Unit 1454: whether the tracked target is out of reach: 50 units away in x, or 50 along the plan direction
// through the owner's rotation; then asks fn_7100985a08. Names and layouts are placeholders.
#include "game/UnkTU1450.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkTrackedActor_b0248 {
    virtual void vf_0x0();
};

struct UnkTrackedPos_b0248;

struct UnkNpc_b0248 {
    u8 pad_0x0[0x3d8];
    s32 state;  // 0x3d8
};

// The object tracked: a position source with a virtual 0x188 that gives a position.
class UnkTarget_b0248 {
public:
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
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual UnkVec3f vf_0x188();
};

struct UnkOwnerTf_b0248 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkPlan_b0248 {
    u8 pad_0x0[0x30];
    UnkOwnerTf_b0248* owner;  // 0x30
    u8 pad_0x38[0x1a0];
    f32 dir[3];  // 0x1d8
};

struct UnkPositionSource_b0249;
struct UnkUnit1452_b56;
struct UnkSmOther_b60;
struct UnkSmBase_b60;
UnkMatrix44 fn_7100078fc8(const UnkMatrix44& m);
bool fn_7100985a08(UnkNpc_b0248* self, UnkTarget_b0248* target);

bool fn_7100984d24(UnkNpc_b0248* self) {
    UnkTarget_b0248* target =
        reinterpret_cast<UnkTarget_b0248*>(fn_710097f1e0(reinterpret_cast<UnkUnit1452_b56*>(self)));
    if (self->state == 2) {
        UnkPlan_b0248* plan = reinterpret_cast<UnkPlan_b0248*>(fn_710097f198(reinterpret_cast<UnkSmBase_b60*>(self)));
        UnkXform* xf = &plan->owner->xform;
        f32 x = xf->m[0][3];
        bool far;
        {
            UnkVec3f pos = target->vf_0x188();
            far = __builtin_fabsf(x - pos.x) >= 50.0f;
        }
        if (!far) {
            UnkVec3f angle = xf->GetAngle();
            UnkMatrix44 m = fn_7100078fc8(reinterpret_cast<const UnkMatrix44&>(angle));
            UnkVec3f col(m.m[2], m.m[6], m.m[10]);
            far = col.x * plan->dir[0] + col.y * plan->dir[1] + col.z * plan->dir[2] >= 50.0f;
        }
        if (far)
            return fn_7100985a08(self, target);
    }
    return false;
}
