#pragma once

#include "types.h"

struct UnkMovedEntry;

struct UnkMovedPairRef {
    u32* value;
    UnkMovedEntry* entry;
};
