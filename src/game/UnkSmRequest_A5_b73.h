#pragma once
// State-machine request template (as src/game/UnkStateMachineSetA6_b62.cpp), shared by the _A5_b73 files.
#include "types.h"
struct UnkSmState_A5_b73 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    virtual void vf_0x20(f32 dt);        // updated
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_A5_b73* __attribute__((aligned(4))) UnkSmStatePtr_A5_b73;

struct UnkSmMachine_A5_b73 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_A5_b73* previous, UnkSmState_A5_b73* next);
    virtual void vf_0x28(UnkSmState_A5_b73* current, UnkSmState_A5_b73* next);

    u64 field_0x8;
    UnkSmState_A5_b73* current;
    UnkSmState_A5_b73* previous;
    u32 count;
    UnkSmStatePtr_A5_b73 states[16];
    u32 pending;
    bool busy;
};

template <u32 Offset>
struct UnkSmOwner_A5_b73 {
    u8 pad_0x0[Offset];
    UnkSmMachine_A5_b73 machine;
};

// Pin: the states array's address is not derived from another one the caller computed.
template <bool Pin>
__attribute__((always_inline)) static inline bool FindState_A5_b73(UnkSmMachine_A5_b73* self, u32 id,
                                                               UnkSmState_A5_b73*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_A5_b73);
    asm("" : "+r"(n));
    UnkSmStatePtr_A5_b73* p = self->states;
    if (Pin)
        asm("" : "+r"(p));
    for (; n != 0; ++p, n -= sizeof(UnkSmStatePtr_A5_b73)) {
        if ((*p)->id == id) {
            out = *p;
            return out != nullptr;
        }
    }
    out = nullptr;
    return false;
}

// Switches to state `id` (the b59 "apply pending" body) and clears the pending id.
template <u32 None, bool Pin = true>
__attribute__((always_inline)) static inline void SwitchState_A5_b73(UnkSmMachine_A5_b73* self, u32 id,
                                                                 UnkSmState_A5_b73* current) {
    if (!current || current->id != id) {
        UnkSmState_A5_b73* found;
        UnkSmState_A5_b73* next;
        u32 enter;
        if (FindState_A5_b73<Pin>(self, id, found)) {
            next = found;
            enter = true;
        } else {
            if (id != 0)
                goto done;
            enter = false;
            asm("" : "+r"(enter));
            next = nullptr;
            asm("" : "+r"(next));
        }
        if (current) {
            current->vf_0x18(id);
            self->vf_0x28(self->current, next);
        }
        self->previous = self->current;
        self->current = found;
        if (enter) {
            typedef void (*EnteredFn)(UnkSmState_A5_b73*, s32);
            s32 previous_id = self->previous ? self->previous->id : None;
            EnteredFn entered = (*reinterpret_cast<EnteredFn**>(found))[2];
            entered(next, previous_id);
            self->vf_0x20(self->previous, next);
        }
    }
done:
    self->pending = None;
}

template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void RequestState_A5_b73(UnkSmMachine_A5_b73* self, u32 id) {
    // Stored through a plain u32 pointer, as in the template's original.
    u32* pending = &self->pending;
    *pending = id;
    if (id > Last)
        return;
    if (self->busy)
        return;
    SwitchState_A5_b73<None>(self, id, self->current);
}

