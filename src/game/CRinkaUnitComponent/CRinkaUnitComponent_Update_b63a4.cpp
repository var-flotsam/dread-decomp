// Component updates that run the base update (fn_71010c424c) and then their small state machine (the
// update of src/game/UnkStateMachineSet_b61.cpp, fn_7100a0a584, whose template is copied here):
// applies the pending state, then updates the current state and applies what it asked for, at most ten
// times, while marked busy. Each one's own unit. Names are placeholders.
#include "types.h"

struct UnkSmState_B63a4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    virtual void vf_0x20(f32 dt);        // updated
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_B63a4* __attribute__((aligned(4))) UnkSmStatePtr_B63a4;

struct UnkSmMachine_B63a4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_B63a4* previous, UnkSmState_B63a4* next);
    virtual void vf_0x28(UnkSmState_B63a4* current, UnkSmState_B63a4* next);

    u64 field_0x8;
    UnkSmState_B63a4* current;
    UnkSmState_B63a4* previous;
    u32 count;
    UnkSmStatePtr_B63a4 states[16];
    u32 pending;
    bool busy;
};

template <u32 Offset>
struct UnkSmOwner_B63a4 {
    u8 pad_0x0[Offset];
    UnkSmMachine_B63a4 machine;
};

__attribute__((always_inline)) static inline bool FindState_B63a4(UnkSmMachine_B63a4* self, u32 id,
                                                               UnkSmState_B63a4*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_B63a4);
    asm("" : "+r"(n));
    for (UnkSmStatePtr_B63a4* p = self->states; n != 0; ++p, n -= sizeof(UnkSmStatePtr_B63a4)) {
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
__attribute__((always_inline)) static inline void SwitchState_B63a4(UnkSmMachine_B63a4* self, u32 id,
                                                                 UnkSmState_B63a4* current) {
    if (!current || current->id != id) {
        UnkSmState_B63a4* found;
        UnkSmState_B63a4* next;
        u32 enter;
        if (FindState_B63a4(self, id, found)) {
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
            typedef void (*EnteredFn)(UnkSmState_B63a4*, s32);
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
__attribute__((always_inline)) static inline void Apply_B63a4(UnkSmMachine_B63a4* self) {
    u32 id = self->pending;
    if (id > Last)
        return;
    SwitchState_B63a4<None>(self, id, self->current);
}

template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void Update_B63a4(UnkSmMachine_B63a4* self, f32 dt) {
    self->busy = true;
    if (self->pending != None)
        Apply_B63a4<Last, None>(self);
    u32 i = 0;
    while (UnkSmState_B63a4* current = self->current) {
        current->vf_0x20(dt);
        if (self->pending == None)
            break;
        Apply_B63a4<Last, None>(self);
        if (i++ > 8)
            break;
    }
    self->busy = false;
}


char* fn_71010c4234(char*);

void fn_710065fb20(UnkSmOwner_B63a4<0x1a8>* self, f32 dt) {
    fn_71010c4234(reinterpret_cast<char*>(self));
    Update_B63a4<3, 4>(&self->machine, dt);
}
