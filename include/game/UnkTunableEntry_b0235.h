#pragma once

#include "types.h"

// A registered tunable: its vf_0x18 prepares it; its flag at +0x20 marks it as read.
struct UnkTunableEntry_b0235 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    u8 pad_0x8[0x18];
    u8 field_0x20;
};
