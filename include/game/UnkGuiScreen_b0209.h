#pragma once

#include "game/UnkGuiLayout_b0209.h"
#include "game/UnkGuiWidget_b0209.h"
#include "types.h"

struct UnkGuiScreen_b0209 {
    u8 pad_0x0[0x40];
    UnkGuiLayout_b0209* layouts;  // 0x40
    u8 pad_0x48[8];
    u32 layout_count;  // 0x50
    u8 pad_0x54[0x14];
    UnkGuiWidget_b0209* target;  // 0x68
    u8 pad_0x70[0xd28];
    f32 field_0xd98;
    f32 field_0xd9c;
    f32 scale_x;  // 0xda0
    f32 scale_y;  // 0xda4
};
