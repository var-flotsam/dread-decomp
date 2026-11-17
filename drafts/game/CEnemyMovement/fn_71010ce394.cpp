// CEnemyMovement unit: updates the movement's state machine. Applies a pending state request (0 to 4;
// 5 is none), then updates the current state, applying the requests it makes, at most ten times.
// Names and layouts are placeholders.
#include "types.h"

struct UnkMoveState_l0ce394 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    virtual void vf_0x20(f32 dt);        // updated
    u64 field_0x8;
    u32 id;
};

struct UnkMoveStateMachine_l0ce394 {
    static const u32 kLast = 4;
    static const u32 kNone = 5;

    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkMoveState_l0ce394* previous, UnkMoveState_l0ce394* next);  // after entering
    virtual void vf_0x28(UnkMoveState_l0ce394* current, UnkMoveState_l0ce394* next);   // after leaving

    u64 field_0x8;
    UnkMoveState_l0ce394* current;   // 0x10
    UnkMoveState_l0ce394* previous;  // 0x18
    u32 count;                        // 0x20
    u8 storage[16 * sizeof(UnkMoveState_l0ce394*)];  // 0x24, the states (only 4-aligned after the count)
    u32 pending;                      // 0xa4
    bool busy;                        // 0xa8

    UnkMoveState_l0ce394** states() { return reinterpret_cast<UnkMoveState_l0ce394**>(storage); }

    UnkMoveState_l0ce394* FindState(u32 state_id) {
        for (UnkMoveState_l0ce394 **p = states(), **end = p + count; p != end; ++p) {
            if ((*p)->id == state_id)
                return *p;
        }
        return nullptr;
    }

    // Leaves the current state for `state_id`'s; state 0 with no such state leaves it for none.
    void SwitchState(u32 state_id) {
        UnkMoveState_l0ce394* cur = current;
        if (cur && cur->id == state_id)
            return;
        UnkMoveState_l0ce394* next = FindState(state_id);
        if (!next && state_id != 0)
            return;
        if (cur) {
            cur->vf_0x18(state_id);
            vf_0x28(current, next);
        }
        previous = current;
        current = next;
        if (next) {
            next->vf_0x10(previous ? previous->id : kNone);
            vf_0x20(previous, next);
        }
    }

    void ApplyPending() {
        u32 state_id = pending;
        if (state_id > kLast)
            return;
        SwitchState(state_id);
        pending = kNone;
    }
};

void fn_71010ce394(UnkMoveStateMachine_l0ce394* self, f32 dt) {
    self->busy = true;
    self->ApplyPending();
    u32 updates = 0;
    while (UnkMoveState_l0ce394* state = self->current) {
        state->vf_0x20(dt);
        if (self->pending == UnkMoveStateMachine_l0ce394::kNone)
            break;
        self->ApplyPending();
        if (updates++ > 8)
            break;
    }
    self->busy = false;
}
