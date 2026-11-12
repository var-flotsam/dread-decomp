// Unit 36 of tools/tu_map.py: a state handler of the NFP worker's machine (g_unk_7101a19948, run by
// fn_710002c1b0). It waits for the worker's answer event without blocking and, once it fired,
// reads the answer and moves the machine on. Names and layouts are placeholders.
#include <nn/os.h>

#include "UnkUnit36_Nfp.h"
#include "game/UnkTU36.h"
#include "types.h"

// The answer to the first request: keep it and the state it came in, then go to state 8.
void fn_710002b7c0() {
    UnkUnit36Request& request = g_unk_7101d4f508;
    if (nn::os::TryWaitEvent(UnkUnit36RequestEvent(request))) {
        u32 result = fn_710002d320(fn_710002cc34());
        UnkUnit36Machine& machine = UnkUnit36GetMachine();
        request.field_0x38 = result;
        request.field_0x28 = machine.state;
        request.field_0x2c = result == 0 ? 5 : 0x17;
        machine.state = 8;
        machine.field_0x218 = 0;
        machine.field_0x21c = 0;
    }
}
