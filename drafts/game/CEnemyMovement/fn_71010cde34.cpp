// CEnemyMovement unit: updates a state machine (states 0-6, no "none" id of its own): applies the pending
// state, then updates the current state and applies what it asked for, at most ten times, while marked busy.
// Names and layouts are placeholders.
#include "../../../src/game/UnkSmRequest_A5_b73.h"
#include "types.h"

// A state of this machine: updated through its own slot 0x48.
struct UnkMovementState_l0cde34 : UnkSmState_A5_b73 {
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48(f32 dt);  // updated
};

void fn_71010cde34(UnkSmMachine_A5_b73* machine, f32 dt) {
    const u32 none = 0x7fffffff;
    machine->busy = true;
    if (machine->pending != none) {
        u32 id = machine->pending;
        if (id <= 6)
            SwitchState_A5_b73<none, false>(machine, id, machine->current);
    }
    u32 i = 0;
    while (UnkSmState_A5_b73* current = machine->current) {
        static_cast<UnkMovementState_l0cde34*>(current)->vf_0x48(dt);
        if (machine->pending == none)
            break;
        u32 id = machine->pending;
        if (id <= 6)
            SwitchState_A5_b73<none, false>(machine, id, machine->current);
        if (i++ > 8)
            break;
    }
    machine->busy = false;
}
