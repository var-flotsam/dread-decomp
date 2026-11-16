// CEnemyMovement unit: handles a movement request. Requests for state 0 or "none" (5) go to the
// current state; others notify the change callback, go to the requested state and switch the state
// machine at +0x838 to it. Names and layouts are placeholders.
#include "game/CEnemyMovement.h"
#include "types.h"

struct UnkMoveRequest_l0c7e6c {
    u8 pad_0x0[0x38];
    u32 state;
};

struct UnkMoveState_l0c7e6c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(u32 previous);  // entered
    virtual void vf_0x18(u32 next);      // left
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60(UnkMoveRequest_l0c7e6c* request, bool flag);
    u64 field_0x8;
    u32 id;
};

typedef UnkMoveState_l0c7e6c* __attribute__((aligned(4))) UnkMoveStatePtr_l0c7e6c;

struct UnkMoveMachine_l0c7e6c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkMoveState_l0c7e6c* previous, UnkMoveState_l0c7e6c* next);
    virtual void vf_0x28(UnkMoveState_l0c7e6c* current, UnkMoveState_l0c7e6c* next);

    u32 CurrentId() const { return current ? current->id : 5; }

    UnkMoveState_l0c7e6c* Find(u32 state) const {
        for (const UnkMoveStatePtr_l0c7e6c* p = states; p != states + count; ++p) {
            if ((*p)->id == state)
                return *p;
        }
        return nullptr;
    }

    u64 field_0x8;
    UnkMoveState_l0c7e6c* current;
    UnkMoveState_l0c7e6c* previous;
    u32 count;
    UnkMoveStatePtr_l0c7e6c states[16];
    u32 pending;
    bool busy;
};

struct UnkStateChangeCallback_l0c7e6c {
    void* context;
    void (*fn)(void* context, u32 from, u32 to);
};

struct UnkEnemyMovement_l0c7e6c {
    u8 pad_0x0[0x838];
    UnkMoveMachine_l0c7e6c machine;
    u8 pad_0x8e8[0x19d0 - 0x8e8];
    UnkStateChangeCallback_l0c7e6c on_state_change;  // 0x19d0
};
static_assert(__builtin_offsetof(UnkEnemyMovement_l0c7e6c, machine.pending) == 0x8dc, "layout");
static_assert(__builtin_offsetof(UnkEnemyMovement_l0c7e6c, on_state_change) == 0x19d0, "layout");

void fn_71010c7e6c(UnkEffectSink* sink, void* params, u32 flags) {
    auto* self = reinterpret_cast<UnkEnemyMovement_l0c7e6c*>(sink);
    auto* request = static_cast<UnkMoveRequest_l0c7e6c*>(params);
    bool flag = flags & 1;
    UnkMoveMachine_l0c7e6c* machine = &self->machine;
    u32 state = request->state;
    bool own_state = state != 0 && state != 5;
    if (!own_state) {
        // No state of its own: the current state handles it.
        if (UnkMoveState_l0c7e6c* current = machine->current)
            current->vf_0x60(request, flag);
        return;
    }
    if (self->on_state_change.fn && machine->CurrentId() != state)
        self->on_state_change.fn(self->on_state_change.context, machine->CurrentId(), state);
    if (state <= 4) {
        if (UnkMoveState_l0c7e6c* target = machine->Find(state))
            target->vf_0x60(request, flag);
    }

    // Switch the state machine to the requested state.
    machine->pending = state;
    if (machine->busy || state > 4)
        return;
    UnkMoveState_l0c7e6c* old = machine->current;
    if (!old || old->id != state) {
        UnkMoveState_l0c7e6c* next = machine->Find(state);
        if (next || state == 0) {
            if (old) {
                old->vf_0x18(state);
                machine->vf_0x28(machine->current, next);
            }
            machine->previous = machine->current;
            machine->current = next;
            if (next) {
                next->vf_0x10(machine->previous ? machine->previous->id : 5);
                machine->vf_0x20(machine->previous, next);
            }
        }
    }
    machine->pending = 5;
}
