#pragma once

#include "game/UnkU64P_b0237.h"

// The list handed to fn_71001cdba4: a count, packed 8-byte entries and a trailing pointer.
struct UnkBindList_b0237 {
    u32 count;
    UnkU64P_b0237 items[16];
    void* tail;
};
