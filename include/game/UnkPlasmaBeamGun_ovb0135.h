#pragma once

#include "types.h"

// The plasma beam's effects: registered with the model manager's effect cache once (flag at +0x1870)
// before the base's step (fn_71008548bc), released again before the other base step (fn_7100854db8).
struct UnkPlasmaBeamGun_ovb0135 {
    u8 pad_0x0[0x1870];
    bool fx_registered;  // 0x1870
};
