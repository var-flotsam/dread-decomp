#pragma once

#include "types.h"

// The area the item is placed in: its size at +0x14.
struct UnkGuiArea {
    u8 pad_0x0[0x14];
    f32 width;
    f32 height;
};
