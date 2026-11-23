#pragma once

// The tunables object UnkProvider::vf_0xd8 returns: three floats at +0xf4 (fn_7100552d94) and three at +0x108
// (fn_7100552e3c). Layout and name are placeholders.
#include "types.h"

struct UnkTunables {
    u8 pad_0x0[0xf4];
    f32 field_0xf4;
    f32 field_0xf8;
    f32 field_0xfc;
    u8 pad_0x100[0x108 - 0x100];
    f32 field_0x108;
    f32 field_0x10c;
    f32 field_0x110;
};
