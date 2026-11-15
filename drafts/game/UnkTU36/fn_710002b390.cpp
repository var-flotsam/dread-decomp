// Unit 36 of tools/tu_map.py: a state handler of the NFP worker's machine. Once the worker's event fired it
// picks the next state and result code from the request's word at +0x2e4. Names and layouts are placeholders.
#include <nn/os.h>

#include "../../base/UnkUnit36/UnkUnit36_Nfp.h"
#include "game/UnkTU36.h"
#include "types.h"

void fn_710002b390() {
    UnkUnit36Request& request = g_unk_7101d4f508;
    if (nn::os::TryWaitEvent(UnkUnit36RequestEvent(request))) {
        UnkUnit36Machine& machine = UnkUnit36GetMachine();
        u32 kind = request.field_0x2e4;
        if (kind < 3) {
            machine.state = 9;
            machine.field_0x218 = 0;
            machine.field_0x21c = 0;
        } else {
            machine.field_0x218 = 0;
            machine.field_0x21c = 0;
            machine.state = 0x17;
            if (kind == 5) {
                machine.field_0x8 = 0x8073;
            } else if (kind == 3) {
                machine.field_0x8 = 0xc073;
            } else {
                machine.field_0x8 = 0x8073;
            }
        }
    }
}
