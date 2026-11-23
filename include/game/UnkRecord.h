#pragma once

#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct __attribute__((packed)) UnkRecord {
    UnkRefObject* field_0x0;
    UnkRefObject* field_0x8;
    u8 pad_0x10[0x38];
};
