// CLifeComponent unit: changing the state at +0x70 and, on entering state 1 or 2, notifying the
// component through vtable slot 0x1c8 (with the old state) or 0x1c0 (with the
// old state and the caller's argument). Names are placeholders.
#include "types.h"

struct UnkLifeState {
    void** vtable;
    u8 pad_0x8[0x68];
    u32 state;  // 0x70

    void vf_0x1c0(u32 old, void* arg) {
        reinterpret_cast<void (*)(UnkLifeState*, u32, void*)>(vtable[0x1c0 / 8])(this, old, arg);
    }
    void vf_0x1c8(u32 old) { reinterpret_cast<void (*)(UnkLifeState*, u32)>(vtable[0x1c8 / 8])(this, old); }
};

void fn_7101160e30(UnkLifeState* self, u32 state, void* arg) {
    u32 old = self->state;
    if (old == state)
        return;
    self->state = state;
    if (state == 1)
        self->vf_0x1c8(old);
    else if (state == 2)
        self->vf_0x1c0(old, arg);
}
