#pragma once

#include "types.h"

// A word and a half-word, copied as one 12-byte block (packed: the copy skips the tail padding).
struct __attribute__((packed)) UnkTU2640PathData {
    u64 field_0x0;
    u32 field_0x8;
};
