// behaviortree::CIsTargetInsideAreaOfInterestCondition unit (tools/tu_map.py, the name from reflection): true
// when the area that fn_7101102f3c finds for the owner's (+0x18) AI component (+0x240) contains (its virtual at
// vtable +0x50) the x and y of the position of the owner's target actor (fn_7100d7c990). Names and layouts are
// placeholders.
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkAreaOfInterest_b67a2 {
    void** vtable;

    bool vf_0x50(const CVector2D* point) {
        return reinterpret_cast<bool (*)(UnkAreaOfInterest_b67a2*, const CVector2D*)>(vtable[0x50 / 8])(this, point);
    }
};

UnkAreaOfInterest_b67a2* fn_7101102f3c(void* ai);

struct UnkAreaOfInterestCondition_b67a2 {
    u8 pad_0x0[0x18];
    UnkActor* owner;
};

bool fn_710039ea88(UnkAreaOfInterestCondition_b67a2* self) {
    UnkAreaOfInterest_b67a2* area = fn_7101102f3c(self->owner->field_0x240);
    UnkActor* target = fn_7100d7c990(self->owner);
    CVector2D point;
    f32 x = target->field_0x44;
    f32 y = target->field_0x54;
    f32 z = target->field_0x64;
    CVector3D position;
    position.x = x;
    position.y = y;
    position.z = z;
    fn_710007b430(&point, &position);
    return area->vf_0x50(&point);
}
