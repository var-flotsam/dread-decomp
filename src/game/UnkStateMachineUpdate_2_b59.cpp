// More instances of the state machine's "apply the pending state" step of
// src/game/UnkStateMachineUpdate_b59.cpp (the same template), one per owner
// unit, among them CSamusGrappleMovementState's (fn_710099c93c). Names are placeholders.
#include "types.h"

struct UnkSmState_b59b {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_b59b* __attribute__((aligned(4))) UnkSmStatePtr_b59b;

struct UnkSmMachine_b59b {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_b59b* previous, UnkSmState_b59b* next);
    virtual void vf_0x28(UnkSmState_b59b* current, UnkSmState_b59b* next);

    u64 field_0x8;
    UnkSmState_b59b* current;
    UnkSmState_b59b* previous;
    u32 count;
    UnkSmStatePtr_b59b states[16];
    u32 pending;
};

__attribute__((always_inline)) static inline bool FindState_b59b(UnkSmMachine_b59b* self, u32 id,
                                                               UnkSmState_b59b*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_b59b);
    asm("" : "+r"(n));
    for (UnkSmStatePtr_b59b* p = self->states; n != 0; ++p, n -= sizeof(UnkSmStatePtr_b59b)) {
        if ((*p)->id == id) {
            out = *p;
            return out != nullptr;
        }
    }
    out = nullptr;
    return false;
}

template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void ApplyPending_b59b(UnkSmMachine_b59b* self) {
    u32 id = self->pending;
    if (id > Last)
        return;
    UnkSmState_b59b* current = self->current;
    if (!current || current->id != id) {
        UnkSmState_b59b* found;
        UnkSmState_b59b* next;
        u32 enter;
        if (FindState_b59b(self, id, found)) {
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
            // The original reads the entered slot from the stored pointer and calls it on `next`
            // (the same object on this path).
            typedef void (*EnteredFn)(UnkSmState_b59b*, s32);
            s32 previous_id = self->previous ? self->previous->id : None;
            EnteredFn entered = (*reinterpret_cast<EnteredFn**>(found))[2];
            entered(next, previous_id);
            self->vf_0x20(self->previous, next);
        }
    }
done:
    self->pending = None;
}

#define UNK_SM_APPLY_B59B(addr, last, none) \
    void fn_##addr(UnkSmMachine_b59b* self) { ApplyPending_b59b<last, none>(self); }

UNK_SM_APPLY_B59B(71002fab10, 6, 7)
UNK_SM_APPLY_B59B(710031df20, 3, 0x7fffffff)
UNK_SM_APPLY_B59B(71003324c4, 5, 6)
UNK_SM_APPLY_B59B(710033f82c, 3, 4)
UNK_SM_APPLY_B59B(710035480c, 4, 5)
UNK_SM_APPLY_B59B(71003c28dc, 6, 7)
UNK_SM_APPLY_B59B(71003fe824, 6, 0x7fffffff)
UNK_SM_APPLY_B59B(71004349fc, 7, 0x7fffffff)
UNK_SM_APPLY_B59B(710044303c, 0xc, 0x7fffffff)
UNK_SM_APPLY_B59B(710048c004, 0xc, 0x7fffffff)
UNK_SM_APPLY_B59B(71004c939c, 4, 5)
UNK_SM_APPLY_B59B(7100510eb8, 0xd, 0xe)
UNK_SM_APPLY_B59B(71005c3e6c, 6, 7)
UNK_SM_APPLY_B59B(7100661238, 3, 4)
UNK_SM_APPLY_B59B(7100987fe8, 2, 3)
UNK_SM_APPLY_B59B(710099c93c, 4, 5)
UNK_SM_APPLY_B59B(71009f11e4, 0xe, 0xf)
UNK_SM_APPLY_B59B(7100a0d718, 7, 6)
UNK_SM_APPLY_B59B(7100a74160, 4, 5)
UNK_SM_APPLY_B59B(7100a883c0, 5, 6)
UNK_SM_APPLY_B59B(7100aa4678, 7, 8)
UNK_SM_APPLY_B59B(7100ab934c, 2, 0x7fffffff)
UNK_SM_APPLY_B59B(7100f147c8, 3, 0x7fffffff)
UNK_SM_APPLY_B59B(7100fd0f34, 7, 0x7fffffff)
UNK_SM_APPLY_B59B(7100fde5bc, 7, 0x7fffffff)
