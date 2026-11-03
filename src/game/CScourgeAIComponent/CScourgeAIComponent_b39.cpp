// CScourgeAIComponent unit (tools/tu_map.py): whether the target is within the range that the object at
// +0x14738 gives for the actor (vtable slot 0x98); true without a target or that object. Names and
// layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkScourgeRange_sca {
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
    virtual f32 vf_0x98(UnkActor* actor);
};

struct UnkScourgeAI_sca {
    u8 pad_0x0[0x30];
    UnkActor* actor;  // 0x30
    u8 pad_0x38[0x14738 - 0x38];
    UnkScourgeRange_sca* range;  // 0x14738
};

bool fn_71006a07e0(UnkScourgeAI_sca* self) {
    UnkActor* target = fn_7100d7c990(self->actor);
    if (target) {
        if (UnkScourgeRange_sca* range = self->range) {
            f32 r = range->vf_0x98(self->actor);
            f32 r2 = r * r;
            UnkActor* actor = self->actor;
            base::math::CVector3D a = {actor->field_0x44, actor->field_0x54, actor->field_0x64};
            base::math::CVector3D b = {target->field_0x44, target->field_0x54, target->field_0x64};
            base::math::CVector3D d = {a.x - b.x, a.y - b.y, a.z - b.z};
            return !(d.x * d.x + d.y * d.y + d.z * d.z > r2);
        }
    }
    return true;
}
