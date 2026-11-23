#pragma once

#include "types.h"

struct UnkKeyNode {
    u64 key;
    u8 field_0x8[0x18];
    UnkKeyNode* next;
};
