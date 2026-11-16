// CEnemyMovement unit: updates a state machine (states 0-6, no "none" id of its own): applies the pending
// state, then updates the current state and applies what it asked for, at most ten times, while marked busy.
// Names and layouts are placeholders.
#include "../UnkSmRequest_A5_b73.h"
#include "types.h"

void fn_71010ce104(UnkSmMachine_A5_b73* machine, f32 dt) {
    const u32 none = 0x7fffffff;
    machine->busy = true;
    if (machine->pending != none) {
        u32 id = machine->pending;
        if (id <= 6)
            SwitchState_A5_b73<none, false>(machine, id, machine->current);
    }
    u32 i = 0;
    while (UnkSmState_A5_b73* current = machine->current) {
        current->vf_0x20(dt);
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
