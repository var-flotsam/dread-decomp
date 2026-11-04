// CAbilityShinespark's unit (tools/tu_map.py unit 433): asks the ability's state machine (+0x6a0) for
// state 2, unless it is already there, in which case fn_710032dfdc runs instead. The request is the
// template of src/game/UnkStateMachineSet_b60.cpp (copied here; "none" is 6). Names are placeholders.
#include "types.h"

struct UnkSmState_A2_b71 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_A2_b71* __attribute__((aligned(4))) UnkSmStatePtr_A2_b71;

struct UnkSmMachine_A2_b71 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_A2_b71* previous, UnkSmState_A2_b71* next);
    virtual void vf_0x28(UnkSmState_A2_b71* current, UnkSmState_A2_b71* next);

    u64 field_0x8;
    UnkSmState_A2_b71* current;
    UnkSmState_A2_b71* previous;
    u32 count;
    UnkSmStatePtr_A2_b71 states[16];
    u32 pending;
    bool busy;
};

template <u32 Offset>
struct UnkSmOwner_A2_b71 {
    u8 pad_0x0[Offset];
    UnkSmMachine_A2_b71 machine;
};

__attribute__((always_inline)) static inline bool FindState_A2_b71(UnkSmMachine_A2_b71* self, u32 id,
                                                               UnkSmState_A2_b71*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_A2_b71);
    asm("" : "+r"(n));
    for (UnkSmStatePtr_A2_b71* p = self->states; n != 0; ++p, n -= sizeof(UnkSmStatePtr_A2_b71)) {
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
__attribute__((always_inline)) static inline void SwitchState_A2_b71(UnkSmMachine_A2_b71* self, u32 id,
                                                                 UnkSmState_A2_b71* current) {
    if (!current || current->id != id) {
        UnkSmState_A2_b71* found;
        UnkSmState_A2_b71* next;
        u32 enter;
        if (FindState_A2_b71(self, id, found)) {
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
            typedef void (*EnteredFn)(UnkSmState_A2_b71*, s32);
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
__attribute__((always_inline)) static inline void RequestState_A2_b71(UnkSmMachine_A2_b71* self, u32 id) {
    // Stored through a plain u32 pointer: the original rereads a state id the caller tested after this.
    u32* pending = &self->pending;
    *pending = id;
    if (id > Last)
        return;
    if (self->busy)
        return;
    SwitchState_A2_b71<None>(self, id, self->current);
}

struct UnkShinespark2_b55;
void fn_710032dfdc(UnkShinespark2_b55* self);

void fn_7100328b3c(UnkSmOwner_A2_b71<0x6a0>* self) {
    UnkSmState_A2_b71* current = self->machine.current;
    if (current && current->id == 2) {
        fn_710032dfdc(reinterpret_cast<UnkShinespark2_b55*>(self));
        return;
    }
    RequestState_A2_b71<5, 6>(&self->machine, 2);
}
