// CFlockingSwarmControllerComponent unit (tools/tu_map.py, the name from reflection): fn_710073f7bc changes
// the state at +0x390. Leaving state 3 calls vf_0x2a0 first; entering states 0-3 calls vf_0x260, vf_0x270,
// vf_0x280 or vf_0x290. Names and layouts are placeholders.
#include "types.h"

struct UnkFlockingSwarm_A3_b72 {
    void** vtable;
    u8 pad_0x8[0x388];
    s32 state;  // 0x390

    void Call(u32 offset) { reinterpret_cast<void (*)(UnkFlockingSwarm_A3_b72*)>(vtable[offset / 8])(this); }
};

void fn_710073f7bc(UnkFlockingSwarm_A3_b72* self, s32 state) {
    if (self->state == state)
        return;
    if (self->state == 3)
        self->Call(0x2a0);
    self->state = state;
    switch (state) {
    case 0:
        self->Call(0x260);
        break;
    case 1:
        self->Call(0x270);
        break;
    case 2:
        self->Call(0x280);
        break;
    case 3:
        self->Call(0x290);
        break;
    }
}
