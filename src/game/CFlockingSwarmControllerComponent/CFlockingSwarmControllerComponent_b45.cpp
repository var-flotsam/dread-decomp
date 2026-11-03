// CFlockingSwarmControllerComponent's unit (tools/tu_map.py unit 1158): dispatches on the state at
// +0x390 to one of four virtuals (slots 0x268, 0x278, 0x288, 0x298). Names are placeholders.
#include "types.h"

struct UnkFlockingSwarm_b45a2 {
    void** vtable;
    u8 pad_0x8[0x388];
    s32 state;  // 0x390

    void Call(u32 offset) { reinterpret_cast<void (*)(UnkFlockingSwarm_b45a2*)>(vtable[offset / 8])(this); }
};

void fn_710073d900(UnkFlockingSwarm_b45a2* self) {
    switch (self->state) {
    case 0:
        self->Call(0x268);
        break;
    case 1:
        self->Call(0x278);
        break;
    case 2:
        self->Call(0x288);
        break;
    case 3:
        self->Call(0x298);
        break;
    }
}
