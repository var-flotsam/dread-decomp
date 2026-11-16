// CCharClassMushroomPlatformComponent's unit: updates the platform's small state machine. Applies a
// pending state change, then updates the current state and applies the changes it requests, at most
// ten times. Names and layouts are placeholders.
#include "../UnkSmRequest_A5_b73.h"
#include "types.h"

struct UnkSmMachine_ov110 {
    u8 data[0xb0];
};

typedef UnkSmMachine_A5_b73 UnkMushroomSm;
typedef UnkSmState_A5_b73 UnkMushroomSmState;

static const u32 kLastState = 4;
static const u32 kNoState = 0x7fffffff;

// Whether the machine has a state `id`; `out` is that state, or null.
static inline bool FindState(UnkMushroomSm* self, u32 id, UnkMushroomSmState*& out) {
    UnkSmStatePtr_A5_b73* p = self->states;
    for (u64 size = (u64)self->count * sizeof(*p); size != 0; ++p, size -= sizeof(*p)) {
        if ((*p)->id == id) {
            out = *p;
            return out != nullptr;
        }
    }
    out = nullptr;
    return false;
}

// Leaves the current state for state `id` (none when `id` is 0 and there is no such state).
static inline void SwitchState(UnkMushroomSm* self, u32 id) {
    UnkMushroomSmState* current = self->current;
    if (current && current->id == id)
        return;
    UnkMushroomSmState* found;
    UnkMushroomSmState* next;
    bool enter;
    if (FindState(self, id, found)) {
        enter = true;
        next = found;
    } else {
        if (id != 0)
            return;
        enter = false;
        next = nullptr;
    }
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

static inline void ApplyPending(UnkMushroomSm* self) {
    u32 id = self->pending;
    if (id > kLastState)
        return;
    SwitchState(self, id);
    self->pending = kNoState;
}

void fn_710088af9c(UnkSmMachine_ov110* self_, f32 dt) {
    auto* self = reinterpret_cast<UnkMushroomSm*>(self_);
    self->busy = true;
    ApplyPending(self);
    for (u32 i = 0; self->current; ++i) {
        self->current->vf_0x20(dt);
        if (self->pending == kNoState)
            break;
        ApplyPending(self);
        if (i > 8)
            break;
    }
    self->busy = false;
}
