// CSamusGrappleMovementState's unit (tools/tu_map.py, unit 1461): switches the state machine at +0x188 to
// state 1 ("none" 5) and drops the handle at +0xb18. Names and layouts are placeholders.
#include "../../../src/game/UnkSmRequest_A5_b73.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkGrappleMovement_l99a51c {
    u8 pad_0x0[0x188];
    UnkSmMachine_A5_b73 machine;
    u8 pad_0x238[0xb18 - 0x238];
    UnkWeakHandle* handle;
};
static_assert(__builtin_offsetof(UnkGrappleMovement_l99a51c, handle) == 0xb18, "layout");

void fn_710099a51c(UnkGrappleMovement_l99a51c* self) {
    UnkSmMachine_A5_b73* machine = &self->machine;
    machine->pending = 1;
    if (machine->busy)
        SwitchState_A5_b73<5, false>(machine, 1, machine->current);
    else
        SwitchState_A5_b73<5>(machine, 1, machine->current);
    ResetRef(&self->handle);
}
