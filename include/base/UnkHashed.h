#pragma once

#include "types.h"

struct UnkNode;

struct UnkHashed {
    u8 pad_0x0[0xe0];
    u32 field_0xe0;
    u8 pad_0xe4[4];
    UnkNode* node_0xe8;
    UnkNode* node_0xf0;
};
