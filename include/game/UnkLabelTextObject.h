#pragma once

#include "types.h"

struct UnkLabelTextNode;

struct UnkLabelTextObject {
    u32 bits;  // the frame it was taken for in bits 12 to 15
    u8 unk_0x4[0x117];
    bool field_0x11b;
    u8 unk_0x11c[0x14];
    UnkLabelTextNode* node;  // 0x130
    void* font;              // 0x138
};
