#pragma once

#include "game/UnkHashKeyObj.h"
#include "types.h"

struct UnkHashNode {
    UnkHashKeyObj* key;
    u8 unk_0x8[0x8];
    UnkHashNode* next;
};
