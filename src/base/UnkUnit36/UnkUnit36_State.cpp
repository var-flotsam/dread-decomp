// Unit 36 of tools/tu_map.py (see src/base/UnkUnit36.cpp): one step of a state machine whose state
// lives in g_unk_7101a19948. It counts the step, runs the current state's handler from the table
// g_unk_7101a19c50 with the argument, and when the handler moved to another state, notes the step
// it happened on; then it records the state it ran and clears the word at +0x21c. Names and
// layouts are placeholders.
#include "game/UnkTU36.h"
#include "types.h"

struct UnkStateMachine_710002c1b0 {
    s32 state;
    s32 last_state;
    u8 unk_0x8[4];
    u32 steps;
    u32 entered_step;
    u8 unk_0x14[0x208];
    u32 field_0x21c;
};

struct UnkStateHandler_710002c1b0 {
    const char* name;
    void* (*run)(void* arg);
};

extern UnkStateHandler_710002c1b0 g_unk_7101a19c50[] __attribute__((visibility("hidden")));

void* fn_710002c1b0(void* arg) {
    UnkStateMachine_710002c1b0& machine = g_unk_7101a19948;
    machine.steps++;
    s32 state = machine.state;
    void* result = g_unk_7101a19c50[state].run(arg);
    if (state != machine.state)
        machine.entered_step = machine.steps;
    machine.last_state = state;
    machine.field_0x21c = 0;
    return result;
}
