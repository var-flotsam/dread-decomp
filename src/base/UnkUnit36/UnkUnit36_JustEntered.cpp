// Unit 36 of tools/tu_map.py: whether the state machine in g_unk_7101a19948 (see
// UnkUnit36_State.cpp) is on its first step, or entered its current state on this step. Names and
// layouts are placeholders.
#include "UnkUnit36_Nfp.h"
#include "types.h"

bool fn_710002c180() {
    UnkStateMachine_710002c1b0& machine = g_unk_7101a19948;
    return machine.entered_step == machine.steps || machine.steps == 1;
}
