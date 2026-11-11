// Unit 1691: the component update that runs a small state machine (the template of
// src/game/UnkStateMachineUpdateA5_b62.cpp, copied here) on the object itself: applies the pending
// state, then updates the current state and applies what it asked for, at most ten times, while
// marked busy. Names are placeholders.
#include "types.h"

struct UnkSmState_ovb0262c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    virtual void vf_0x20(f32 dt);        // updated
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_ovb0262c* __attribute__((aligned(4))) UnkSmStatePtr_ovb0262c;

struct UnkSmMachine_ovb0262c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_ovb0262c* previous, UnkSmState_ovb0262c* next);
    virtual void vf_0x28(UnkSmState_ovb0262c* current, UnkSmState_ovb0262c* next);

    u64 field_0x8;
    UnkSmState_ovb0262c* current;
    UnkSmState_ovb0262c* previous;
    u32 count;
    UnkSmStatePtr_ovb0262c states[16];
    u32 pending;
    bool busy;
};

template <u32 Offset>
struct UnkSmOwner_ovb0262c {
    u8 pad_0x0[Offset];
    UnkSmMachine_ovb0262c machine;
};

__attribute__((always_inline)) static inline bool FindState_ovb0262c(UnkSmMachine_ovb0262c* self, u32 id,
                                                               UnkSmState_ovb0262c*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_ovb0262c);
    asm("" : "+r"(n));
    for (UnkSmStatePtr_ovb0262c* p = self->states; n != 0; ++p, n -= sizeof(UnkSmStatePtr_ovb0262c)) {
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
__attribute__((always_inline)) static inline void SwitchState_ovb0262c(UnkSmMachine_ovb0262c* self, u32 id,
                                                                 UnkSmState_ovb0262c* current) {
    if (!current || current->id != id) {
        UnkSmState_ovb0262c* found;
        UnkSmState_ovb0262c* next;
        u32 enter;
        if (FindState_ovb0262c(self, id, found)) {
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
            typedef void (*EnteredFn)(UnkSmState_ovb0262c*, s32);
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
__attribute__((always_inline)) static inline void Apply_ovb0262c(UnkSmMachine_ovb0262c* self) {
    u32 id = self->pending;
    if (id > Last)
        return;
    SwitchState_ovb0262c<None>(self, id, self->current);
}

template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void Update_ovb0262c(UnkSmMachine_ovb0262c* self, f32 dt) {
    self->busy = true;
    if (self->pending != None)
        Apply_ovb0262c<Last, None>(self);
    u32 i = 0;
    while (UnkSmState_ovb0262c* current = self->current) {
        current->vf_0x20(dt);
        if (self->pending == None)
            break;
        Apply_ovb0262c<Last, None>(self);
        // keeps the guard counting from the old value
        asm("" : "+r"(i));
        if (i++ > 8)
            break;
    }
    self->busy = false;
}


void fn_7100b4c684(UnkSmOwner_ovb0262c<0>* self, f32 dt) { Update_ovb0262c<3, 4>(&self->machine, dt); }
