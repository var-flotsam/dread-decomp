// CObsydomithonAIComponent unit (tools/tu_map.py, the name from reflection): fn_710062f7d4 writes the angle in
// degrees, wrapped into [0, 360), from the actor to its target (fn_7100d7c990) moved by the given offset,
// with angles within 0.001 of zero kept at 0.001; false when there is no target. fn_710062f6e4 is the same
// with the component's own offset at +0x1479c. Names are placeholders.
#include "game/UnkActor.h"
#include "types.h"

extern "C" f32 atan2f(f32 y, f32 x);

struct UnkVec2_b69a4 {
    f32 x, y;
};

struct UnkObsydomithonAI_b69a4 {
    u8 pad_0x0[0x30];
    UnkActor* actor;
    u8 pad_0x38[0x14764];
    UnkVec2_b69a4 offset;  // 0x1479c
};

static inline __attribute__((always_inline)) bool AngleToTarget_b69a4(UnkObsydomithonAI_b69a4* self,
                                                                       const UnkVec2_b69a4* offset, f32* angle) {
    UnkActor* target = fn_7100d7c990(self->actor);
    f32 tx = target->field_0x44;
    f32 ty = target->field_0x54;
    f32 x = tx + offset->x;
    f32 y = ty + offset->y;
    UnkActor* actor = self->actor;
    f32 ax = actor->field_0x44;
    f32 ay = actor->field_0x54;
    f32 dx = x - ax;
    f32 dy = y - ay;
    *angle = atan2f(dy, dx) * 57.29578f;
    if (*angle >= 360.0f)
        *angle += -360.0f;
    else if (*angle < -0.001f)
        *angle += 360.0f;
    if (*angle >= -0.001f && *angle <= 0.001f)
        *angle = 0.001f;
    return true;
}

bool fn_710062f6e4(UnkObsydomithonAI_b69a4* self, f32* angle) {
    if (!fn_7100d7c990(self->actor))
        return false;
    return AngleToTarget_b69a4(self, &self->offset, angle);
}

bool fn_710062f7d4(UnkObsydomithonAI_b69a4* self, const UnkVec2_b69a4* offset, f32* angle) {
    if (!fn_7100d7c990(self->actor))
        return false;
    return AngleToTarget_b69a4(self, offset, angle);
}
