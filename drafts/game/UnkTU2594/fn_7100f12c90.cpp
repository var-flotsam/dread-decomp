// Unit 2594: updates a state machine with nine states ("none" is 9). Applies the pending request, then runs
// the current state's update and any state change it requests, at most ten times. Names and layouts are placeholders.
#include "../../../src/game/UnkSmRequest_A5_b73.h"
#include "types.h"

static const u32 kNoState = 9;

void fn_7100f12c90(UnkSmMachine_A5_b73* self, f32 dt) {
    self->busy = true;
    if (self->pending <= 8)
        SwitchState_A5_b73<kNoState>(self, self->pending, self->current);
    u32 i = 0;
    while (UnkSmState_A5_b73* current = self->current) {
        current->vf_0x20(dt);
        u32 id = self->pending;
        if (id == kNoState)
            break;
        if (id <= 8)
            SwitchState_A5_b73<kNoState>(self, id, self->current);
        if (i++ > 8)
            break;
    }
    self->busy = false;
}
