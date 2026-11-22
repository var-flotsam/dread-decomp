#pragma once

#include "game/UnkRegistryString_b0248.h"
#include "types.h"

class UnkRegistryHandler_b0248;

struct UnkRegistryNode_b0248 {
    UnkRegistryString_b0248* key;
    UnkRegistryHandler_b0248* value;  // 0x8
    u8 pad_0x10[0x10];
    UnkRegistryNode_b0248* next;  // 0x20
};
