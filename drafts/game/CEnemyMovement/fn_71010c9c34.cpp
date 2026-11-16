// CEnemyMovement unit: updates the movement's state machine (states 0-4, none 5): applies the pending state,
// then updates the current state and applies what it asked for, at most ten times, while marked busy.
// Names and layouts are placeholders.
#include "../UnkSmRequest_A5_b73.h"
#include "types.h"

struct UnkMovementStateMachine_i;

// A movement state: updated through its own slot 0x48.
struct UnkMovementState_l0c9c34 : UnkSmState_A5_b73 {
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48(f32 dt);  // updated
};

void fn_71010c9c34(UnkMovementStateMachine_i* self, f32 dt) {
    UnkSmMachine_A5_b73* machine = reinterpret_cast<UnkSmMachine_A5_b73*>(self);
    machine->busy = true;
    if (machine->pending != 5) {
        u32 id = machine->pending;
        if (id <= 4)
            SwitchState_A5_b73<5, false>(machine, id, machine->current);
    }
    u32 i = 0;
    while (UnkSmState_A5_b73* current = machine->current) {
        static_cast<UnkMovementState_l0c9c34*>(current)->vf_0x48(dt);
        if (machine->pending == 5)
            break;
        u32 id = machine->pending;
        if (id <= 4)
            SwitchState_A5_b73<5, false>(machine, id, machine->current);
        if (i++ > 8)
            break;
    }
    machine->busy = false;
}
