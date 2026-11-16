// behaviortree::CAutoolGoToRepairPointAction's unit (tools/tu_map.py): after the node's base step, requests
// state 1 when the "v3SmartObjectPos" value is set (fn_710039525c); otherwise picks a hash at +0x3a8 from the
// owner's (+0x18) transform orientation and requests state 2 (state machine at +0x2d0, "none" 7).
// Names and layouts are placeholders.
#include "../../UnkSmRequest_A5_b73.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::global::CStrId;

struct UnkAutoolOwner_l3c0964 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
};

struct UnkAutoolGoToRepairPoint_l3c0964 {
    u8 pad_0x0[0x18];
    UnkAutoolOwner_l3c0964* owner;
    u8 pad_0x20[0x2d0 - 0x20];
    UnkSmMachine_A5_b73 machine;  // 0x2d0
    u8 pad_0x380[0x3a8 - 0x380];
    u64 field_0x3a8;
};
static_assert(__builtin_offsetof(UnkAutoolGoToRepairPoint_l3c0964, field_0x3a8) == 0x3a8, "layout");

void fn_71003c0964(UnkAutoolGoToRepairPoint_l3c0964* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    base::math::CVector3D pos;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    bool found;
    {
        CStrId key("v3SmartObjectPos", true);
        found = fn_710039525c(reinterpret_cast<UnkSelf*>(self), &key, &pos);
    }
    if (found) {
        RequestState_A5_b73<15, 7>(&self->machine, 1);
        return;
    }
    const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
    f32 forward = __builtin_fabsf(m.m[2]);
    f32 side = m.m[6];
    if (forward > __builtin_fabsf(side))
        self->field_0x3a8 = 0x53e18b7229c1d78a;
    else if (side < 0.0f)
        self->field_0x3a8 = 0xc408aa7f5161fff7;
    else
        self->field_0x3a8 = 0x15aa42b142b0b02f;
    RequestState_A5_b73<15, 7>(&self->machine, 2);
}
