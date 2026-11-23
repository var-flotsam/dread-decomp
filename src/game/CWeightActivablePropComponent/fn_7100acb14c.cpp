// A state machine's pending-state switch, one instantiation per state enum (Count states, None
// meaning no state): if a state is pending and is not the current one, the state with its id
// (or none, for state 0 without one) is entered. The current state is left (vf_0x18) and the
// owner told (vf_0x28), then the new state entered with the previous id (vf_0x10) and the owner
// told again (vf_0x20). The pending state is cleared either way.
#include "types.h"

struct UnkState_acb14c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);
    virtual void vf_0x18();
    u64 field_0x8;
    s32 id;
};

// The states are only 4-aligned after the count.
typedef UnkState_acb14c* __attribute__((aligned(4))) UnkStatePtr;

struct UnkStateMachine_acb14c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkState_acb14c* previous, UnkState_acb14c* next);
    virtual void vf_0x28(UnkState_acb14c* current, UnkState_acb14c* next);

    u64 field_0x8;
    UnkState_acb14c* current;
    UnkState_acb14c* previous;
    u32 count;
    UnkStatePtr states[16];
    s32 pending;
};

__attribute__((always_inline)) static inline UnkState_acb14c* FindState(UnkStateMachine_acb14c* self, s32 id) {
    for (UnkStatePtr *p = self->states, *e = p + self->count; p != e; ++p) {
        if ((*p)->id == id)
            return *p;
    }
    return nullptr;
}

// The original keeps whether a state is entered in its own register (and the new state in
// two), so it saves one more register; the logic is the same.
template <u32 Count, s32 None>
__attribute__((always_inline)) static inline void UpdateState(UnkStateMachine_acb14c* self) {
    s32 pending = self->pending;
    if ((u32)pending >= Count)
        return;
    UnkState_acb14c* current = self->current;
    if (!current || current->id != pending) {
        UnkState_acb14c* next = FindState(self, pending);
        if (next || pending == 0) {
            if (current)
                current->vf_0x18();
            self->vf_0x28(self->current, next);
            UnkState_acb14c* prev = self->current;
            self->current = next;
            self->previous = prev;
            if (next) {
                next->vf_0x10(prev ? prev->id : None);
                self->vf_0x20(self->previous, next);
            }
        }
    }
    self->pending = None;
}

// NON_MATCHING(minor): register allocation (see UpdateState).
void fn_7100acb14c(UnkStateMachine_acb14c* self) { UpdateState<3, 3>(self); }
// NON_MATCHING(minor): register allocation (see UpdateState).
void fn_7100b2bb00(UnkStateMachine_acb14c* self) { UpdateState<7, 0x7fffffff>(self); }
// NON_MATCHING(minor): register allocation (see UpdateState).
void fn_7100b486dc(UnkStateMachine_acb14c* self) { UpdateState<5, 0x7fffffff>(self); }
// NON_MATCHING(minor): register allocation (see UpdateState).
void fn_7100dd1250(UnkStateMachine_acb14c* self) { UpdateState<4, 4>(self); }
// NON_MATCHING(minor): register allocation (see UpdateState).
void fn_7100dd8f54(UnkStateMachine_acb14c* self) { UpdateState<9, 9>(self); }
