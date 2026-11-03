// CAbilityOpticCamouflage unit (tools/tu_map.py unit 432): after the base step (fn_71002fa298), when the
// state at +0xb4 is 6 (or 7 with the sub-state at +0x20 in 6), calls vtable slot 0xa0 with whether the
// owner's +0xb8 differs from the global at 0x71726dbf68. Names and layouts are placeholders.
#include "types.h"

struct UnkOpticSubState_b47 {
    u8 pad_0x0[0x10];
    u32 state;  // 0x10
};

struct UnkOpticOwner_b47 {
    u8 pad_0x0[0xb8];
    void* field_0xb8;  // 0xb8
};

struct UnkOpticAbility_b47 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0(bool on);
    u8 pad_0x8[0x18];
    UnkOpticSubState_b47* sub;  // 0x20
    u8 pad_0x28[0x8c];
    u32 state;  // 0xb4
};

extern void* g_unk_71726dbf68;
void fn_71002fa298(UnkOpticAbility_b47* self, UnkOpticOwner_b47* owner);

void fn_7100324284(UnkOpticAbility_b47* self, UnkOpticOwner_b47* owner) {
    fn_71002fa298(self, owner);
    u32 state = self->state;
    if (state == 7) {
        if (!self->sub)
            return;
        state = self->sub->state;
    }
    if (state == 6)
        self->vf_0xa0(owner->field_0xb8 != g_unk_71726dbf68);
}
