// CRedenkiSwarmControllerComponent unit (tools/tu_map.py unit 977). fn_7100650ce4: sets the byte at
// +0x25c, then calls virtual slot 0x2e8 with (0, 0) when the count at +0x530 is set, else slot 0x330.
// Names and layouts are placeholders.
#include "types.h"

#define UNK_VF8(n) virtual void vf_##n##_0(); virtual void vf_##n##_1(); virtual void vf_##n##_2(); \
    virtual void vf_##n##_3(); virtual void vf_##n##_4(); virtual void vf_##n##_5(); \
    virtual void vf_##n##_6(); virtual void vf_##n##_7();

class UnkRedenkiSwarm_b49a3 {
public:
    // Slots 0x0-0x2e0 (93 entries).
    UNK_VF8(0) UNK_VF8(1) UNK_VF8(2) UNK_VF8(3) UNK_VF8(4) UNK_VF8(5) UNK_VF8(6) UNK_VF8(7)
    UNK_VF8(8) UNK_VF8(9) UNK_VF8(10)
    virtual void vf_0x2c0();
    virtual void vf_0x2c8();
    virtual void vf_0x2d0();
    virtual void vf_0x2d8();
    virtual void vf_0x2e0();
    virtual void vf_0x2e8(u32 a, u32 b);
    virtual void vf_0x2f0();
    virtual void vf_0x2f8();
    virtual void vf_0x300();
    virtual void vf_0x308();
    virtual void vf_0x310();
    virtual void vf_0x318();
    virtual void vf_0x320();
    virtual void vf_0x328();
    virtual void vf_0x330();

    u8 pad_0x8[0x254];
    bool field_0x25c;
    u8 pad_0x25d[0x2d3];
    u32 field_0x530;
};

#undef UNK_VF8

void fn_7100650ce4(UnkRedenkiSwarm_b49a3* self) {
    self->field_0x25c = true;
    if (self->field_0x530 != 0)
        self->vf_0x2e8(0, 0);
    else
        self->vf_0x330();
}
