// CCharClassMushroomPlatformComponent's unit: updates the platform's small state machine. Applies a
// pending state change, then updates the current state and applies the changes it requests, at most
// ten times. Names and layouts are placeholders.
#include "../../../src/game/UnkSmRequest_A5_b73.h"
#include "types.h"

struct UnkSmMachine_ov110 {
    u8 data[0xb0];
};

static const u32 kLastState = 4;
static const u32 kNoState = 0x7fffffff;

static inline void ApplyPending(UnkSmMachine_A5_b73* self) {
    u32 id = self->pending;
    if (id > kLastState)
        return;
    SwitchState_A5_b73<kNoState>(self, id, self->current);
}

void fn_710088af9c(UnkSmMachine_ov110* self_, f32 dt) {
    auto* self = reinterpret_cast<UnkSmMachine_A5_b73*>(self_);
    self->busy = true;
    ApplyPending(self);
    u32 i = 0;
    while (UnkSmState_A5_b73* current = self->current) {
        current->vf_0x20(dt);
        if (self->pending == kNoState)
            break;
        ApplyPending(self);
        if (i++ > 8)
            break;
    }
    self->busy = false;
}
