// behaviortree::CIsInHightPositionCondition's unit (tools/tu_map.py unit 890): the condition's check. The
// owner (+0x18) must have a target (fn_7100d7c990). When the owner's up axis (its matrix's +0x14) is level
// within 0.01, true if the target is below the owner's top (y plus the +0x37c height) or within 200 on x;
// otherwise true only when that axis points down and the target is below the top. Names and layouts are
// placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkHighPositionCondition_l5c9708 {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
};

static inline const UnkMatrix44& Matrix(UnkActor* actor) {
    return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
}

bool fn_71005c9708(UnkHighPositionCondition_l5c9708* self) {
    if (!self->owner || !fn_7100d7c990(self->owner))
        return false;
    f32 up = Matrix(self->owner).m[5];
    if (up + 0.01f >= 0.0f && up - 0.01f <= 0.0f) {
        f32 target_y = fn_7100d7c990(self->owner)->field_0x54;
        UnkActor* owner = self->owner;
        f32 top = owner->field_0x54 + owner->field_0x37c;
        UnkActor* target = fn_7100d7c990(owner);
        if (target_y < top)
            return true;
        f32 x = self->owner->field_0x44;
        if (target->field_0x44 < x + 200.0f)
            return true;
        return !(target->field_0x44 <= x - 200.0f);
    }
    if (Matrix(self->owner).m[5] < 0.0f) {
        UnkActor* target = fn_7100d7c990(self->owner);
        UnkActor* owner = self->owner;
        if (target->field_0x54 < owner->field_0x54 + owner->field_0x37c)
            return true;
    }
    return false;
}
