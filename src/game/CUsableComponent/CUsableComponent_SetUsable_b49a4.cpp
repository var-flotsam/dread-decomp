// CUsableComponent unit (tools/tu_map.py): stores the flag at +0xfa when it changes, runs
// fn_7100a9f7f0, then the virtual at +0x2c0 (set) or +0x2c8 (cleared). Names are placeholders.
#include "game/CUsableComponent.h"
#include "types.h"

struct UnkUsableComponent_b49a4 {
    void** vtable;
    u8 pad_0x8[0xf2];
    bool flag;  // +0xfa

    void vf_0x2c0() { reinterpret_cast<void (*)(UnkUsableComponent_b49a4*)>(vtable[0x2c0 / 8])(this); }
    void vf_0x2c8() { reinterpret_cast<void (*)(UnkUsableComponent_b49a4*)>(vtable[0x2c8 / 8])(this); }
};

void fn_7100a9ffcc(UnkUsableComponent_b49a4* self, bool flag) {
    if (self->flag == flag)
        return;
    self->flag = flag;
    fn_7100a9f7f0(reinterpret_cast<char*>(self));
    if (flag)
        self->vf_0x2c0();
    else
        self->vf_0x2c8();
}
