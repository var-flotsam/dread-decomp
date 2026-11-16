// CAbilityGhostAura unit: given a movement state's parameters, hands them to the state they name and
// asks the movement state machine (+0x2cf8) to switch to it. Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

// A movement state's parameters (a CSamusMovementState::CParams): the state they are for.
struct UnkMovementStateParams {
    virtual const base::reflection::CClass* vf_0x0() const;
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual u32 vf_0x18() const;  // the state's id
};

struct UnkMovementState {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(u32 previous);  // entered
    virtual void vf_0x18(u32 next);      // left
    virtual void vf_0x20(f32 dt);
    virtual void vf_0x28();
    virtual void vf_0x30(UnkMovementStateParams* params);  // takes its parameters
    u64 field_0x8;
    u32 id;
};

typedef UnkMovementState* __attribute__((aligned(4))) UnkMovementStatePtr;

struct UnkMovementStateMachine {
    static const u32 kStateCount = 15;
    static const u32 kNone = 15;

    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkMovementState* previous, UnkMovementState* next);
    virtual void vf_0x28(UnkMovementState* current, UnkMovementState* next);

    u64 field_0x8;
    UnkMovementState* current;
    UnkMovementState* previous;
    u32 count;
    UnkMovementStatePtr states[16];
    u32 pending;
    bool busy;

    // Whether a state has this id; `out` is that state, or null.
    bool TryFind(u32 state_id, UnkMovementState*& out) {
        for (UnkMovementStatePtr* it = states; it != states + count; ++it) {
            if ((*it)->id == state_id) {
                out = *it;
                return out != nullptr;
            }
        }
        out = nullptr;
        return false;
    }
};

struct UnkAbilityGhostAura {
    u8 pad_0x0[0x2cf8];
    UnkMovementStateMachine movement;
};
static_assert(__builtin_offsetof(UnkAbilityGhostAura, movement.pending) == 0x2d9c, "UnkAbilityGhostAura placeholder layout");

static void RequestState(UnkMovementStateMachine* machine, u32 state_id) {
    machine->pending = state_id;
    if (machine->busy || state_id >= UnkMovementStateMachine::kStateCount)
        return;
    if (!machine->current || machine->current->id != state_id) {
        UnkMovementState* next;
        bool found = machine->TryFind(state_id, next);
        if (found || state_id == 0) {
            if (machine->current) {
                machine->current->vf_0x18(state_id);
                machine->vf_0x28(machine->current, next);
            }
            machine->previous = machine->current;
            machine->current = next;
            if (found) {
                next->vf_0x10(machine->previous ? machine->previous->id : UnkMovementStateMachine::kNone);
                machine->vf_0x20(machine->previous, next);
            }
        }
    }
    machine->pending = UnkMovementStateMachine::kNone;
}

void fn_710031433c(UnkAbilityGhostAura* self, UnkMovementStateParams* params) {
    if (!fn_710009d50c(params->vf_0x0(), fn_71009b0304()))
        return;
    u32 state_id = params->vf_0x18();
    if (state_id < UnkMovementStateMachine::kStateCount) {
        UnkMovementState* state;
        if (self->movement.TryFind(state_id, state))
            state->vf_0x30(params);
    }
    RequestState(&self->movement, state_id);
}
