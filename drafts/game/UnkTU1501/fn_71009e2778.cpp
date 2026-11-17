// Unit 1501 (tools/tu_map.py, CSamusMovement): updates the movement's state machine at +0x2cf8: applies
// a pending state, then runs the current state's update and any state it requests, up to ten times.
// Names and layouts are placeholders.
#include "types.h"

struct UnkMoveState_l9e2778 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    virtual void vf_0x20(void* arg);     // updated
    u64 field_0x8;
    u32 id;
};

typedef UnkMoveState_l9e2778* __attribute__((aligned(4))) UnkMoveStatePtr_l9e2778;

struct UnkMoveMachine_l9e2778 {
    static const u32 kNone = 15;

    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkMoveState_l9e2778* previous, UnkMoveState_l9e2778* next);
    virtual void vf_0x28(UnkMoveState_l9e2778* current, UnkMoveState_l9e2778* next);

    u64 field_0x8;
    UnkMoveState_l9e2778* current;
    UnkMoveState_l9e2778* previous;
    u32 count;
    UnkMoveStatePtr_l9e2778 states[16];
    u32 pending;
    bool busy;

    UnkMoveState_l9e2778* Find(u32 id) {
        UnkMoveStatePtr_l9e2778* end = states + count;
        for (UnkMoveStatePtr_l9e2778* p = states; p != end; ++p) {
            if ((*p)->id == id)
                return *p;
        }
        return nullptr;
    }

    // Leaves the current state for `id` (none for 0), unless it is already current.
    void SwitchTo(u32 id) {
        UnkMoveState_l9e2778* old = current;
        if (!old || old->id != id) {
            UnkMoveState_l9e2778* next = Find(id);
            if (next || id == 0) {
                if (old) {
                    old->vf_0x18(id);
                    vf_0x28(current, next);
                }
                previous = current;
                current = next;
                if (next) {
                    next->vf_0x10(previous ? previous->id : kNone);
                    vf_0x20(previous, next);
                }
            }
        }
        pending = kNone;
    }
};

struct UnkSamusMovement_l9e2778 {
    u8 pad_0x0[0x2cf8];
    UnkMoveMachine_l9e2778 machine;  // 0x2cf8
};
static_assert(__builtin_offsetof(UnkMoveMachine_l9e2778, pending) == 0xa4, "layout");

void fn_71009e2778(UnkSamusMovement_l9e2778* self, void* arg) {
    UnkMoveMachine_l9e2778& machine = self->machine;
    machine.busy = true;
    if (machine.pending < UnkMoveMachine_l9e2778::kNone)
        machine.SwitchTo(machine.pending);
    for (s32 i = 0; machine.current;) {
        machine.current->vf_0x20(arg);
        u32 id = machine.pending;
        if (id == UnkMoveMachine_l9e2778::kNone)
            break;
        if (id < UnkMoveMachine_l9e2778::kNone)
            machine.SwitchTo(id);
        if (++i == 10)
            break;
    }
    machine.busy = false;
}
