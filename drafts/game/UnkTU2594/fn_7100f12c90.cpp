// Unit 2594: updates a state machine with nine states ("none" is 9). Applies the pending request, then runs
// the current state's update and any state change it requests, at most ten times. Names and layouts are placeholders.
#include "../UnkSmRequest_A5_b73.h"
#include "types.h"

#include <algorithm>

static const u32 kNoState = 9;

static inline UnkSmState_A5_b73* FindState(UnkSmMachine_A5_b73* self, u32 id) {
    UnkSmStatePtr_A5_b73* end = self->states + self->count;
    UnkSmStatePtr_A5_b73* it =
        std::find_if(self->states, end, [id](UnkSmState_A5_b73* state) { return state->id == id; });
    return it != end ? *it : nullptr;
}

static inline void SwitchState(UnkSmMachine_A5_b73* self, u32 id) {
    UnkSmState_A5_b73* current = self->current;
    if (!current || current->id != id) {
        UnkSmState_A5_b73* found = FindState(self, id);
        bool enter = found != nullptr;
        UnkSmState_A5_b73* next = enter ? found : nullptr;
        if (enter || id == 0) {
            if (current) {
                current->vf_0x18(id);
                self->vf_0x28(self->current, next);
            }
            self->previous = self->current;
            self->current = found;
            if (enter) {
                next->vf_0x10(self->previous ? self->previous->id : kNoState);
                self->vf_0x20(self->previous, next);
            }
        }
    }
    self->pending = kNoState;
}

void fn_7100f12c90(UnkSmMachine_A5_b73* self, f32 dt) {
    self->busy = true;
    if (self->pending <= 8)
        SwitchState(self, self->pending);
    for (u32 i = 0; i < 10; ++i) {
        UnkSmState_A5_b73* current = self->current;
        if (!current)
            break;
        current->vf_0x20(dt);
        u32 id = self->pending;
        if (id == kNoState)
            break;
        if (id <= 8)
            SwitchState(self, id);
    }
    self->busy = false;
}
