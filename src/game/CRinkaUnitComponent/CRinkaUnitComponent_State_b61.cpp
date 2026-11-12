// CRinkaUnitComponent asks its state machine (the machine of src/game/UnkStateMachineUpdate_b59.cpp at +0x1a8)
// for a new state after storing its parameters. The request template is a copy of
// src/game/UnkStateMachineSet_b60.cpp's. Names are placeholders.
#include "types.h"

struct UnkSmState_b61 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_b61* __attribute__((aligned(4))) UnkSmStatePtr_b61;

struct UnkSmMachine_b61 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_b61* previous, UnkSmState_b61* next);
    virtual void vf_0x28(UnkSmState_b61* current, UnkSmState_b61* next);

    u64 field_0x8;
    UnkSmState_b61* current;
    UnkSmState_b61* previous;
    u32 count;
    UnkSmStatePtr_b61 states[16];
    u32 pending;
    bool busy;
};

template <u32 Offset>
struct UnkSmOwner_b61 {
    u8 pad_0x0[Offset];
    UnkSmMachine_b61 machine;
};

__attribute__((always_inline)) static inline bool FindState_b61(UnkSmMachine_b61* self, u32 id,
                                                               UnkSmState_b61*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_b61);
    asm("" : "+r"(n));
    for (UnkSmStatePtr_b61* p = self->states; n != 0; ++p, n -= sizeof(UnkSmStatePtr_b61)) {
        if ((*p)->id == id) {
            out = *p;
            return out != nullptr;
        }
    }
    out = nullptr;
    return false;
}

// Switches to state `id` (the b59 "apply pending" body) and clears the pending id.
template <u32 None>
__attribute__((always_inline)) static inline void SwitchState_b61(UnkSmMachine_b61* self, u32 id,
                                                                 UnkSmState_b61* current) {
    if (!current || current->id != id) {
        UnkSmState_b61* found;
        UnkSmState_b61* next;
        u32 enter;
        if (FindState_b61(self, id, found)) {
            next = found;
            enter = true;
        } else {
            if (id != 0)
                goto done;
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
            typedef void (*EnteredFn)(UnkSmState_b61*, s32);
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
__attribute__((always_inline)) static inline void RequestState_b61(UnkSmMachine_b61* self, u32 id) {
    // Stored through a plain u32 pointer: the original rereads a state id the caller tested after this.
    u32* pending = &self->pending;
    *pending = id;
    if (id > Last)
        return;
    if (self->busy)
        return;
    SwitchState_b61<None>(self, id, self->current);
}

struct UnkRinkaUnit_b61 {
    u8 pad_0x0[0x48];
    u32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    u8 pad_0x54[0x1a8 - 0x54];
    UnkSmMachine_b61 machine;
};

void fn_710065ff7c(UnkRinkaUnit_b61* self, u32 a) {
    self->field_0x48 = self->field_0x4c;
    self->field_0x50 = a;
    RequestState_b61<3, 4>(&self->machine, 2);
}

void fn_710066008c(UnkRinkaUnit_b61* self, u32 a, u32 b) {
    self->field_0x50 = a;
    self->field_0x48 = b;
    RequestState_b61<3, 4>(&self->machine, 2);
}

void fn_7100660198(UnkRinkaUnit_b61* self) {
    self->field_0x48 = 0x7fffffff;
    RequestState_b61<3, 4>(&self->machine, 1);
}

