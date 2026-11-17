// Resets an attack state's cached values: looks up the owner's body and its field at +0x2b0 (or
// null) for a query, runs the owner's attack lookup (with a fixed key) when a flag is set, sets four
// floats to -1 and continues into the state's next step with the limit read from a member object
// (or INT_MAX when there is none; the step, fn_71004321a0, takes a float only when there is one).
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/CChozoCommanderKiGrabAttack.h"
#include "game/UnkKiGrabAttack.h"
#include "game/UnkTU437.h"
#include "types.h"

bool fn_71004333a8(UnkKiGrabAttack* self) {
    UnkActor* body = fn_7100d7c990(self->owner);
    fn_7100352b14(body ? body->field_0x2b0 : nullptr, self->owner, 1);
    if (self->field_0x13c) {
        f32 value = fn_7101177ea0(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), 0xf4061ff25a2163d6ull);
        self->field_0x140 = 0.0f;
        self->field_0x144 = -1.0f;
        self->field_0x148 = -1.0f;
        self->field_0x14c = -1.0f;
        u32 limit = self->limit ? self->limit->limit : 0x7fffffff;
        return fn_71004321a0(self, limit, value);
    }
    self->field_0x140 = 0.0f;
    self->field_0x144 = -1.0f;
    self->field_0x148 = -1.0f;
    self->field_0x14c = -1.0f;
    u32 limit = self->limit ? self->limit->limit : 0x7fffffff;
    return fn_71004321a0(self, limit, -1.0f);
}
