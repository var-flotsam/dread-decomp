#pragma once

#include "types.h"

// The six id lists fn_7100ee7134 empties (src/game_hi/fn_7100ee7134.cpp: two string-id lists and
// four file-path-id lists); only the counts are read here.
// The six id lists a sub-area change fills; only the counts are read here.
struct UnkIdList {
    void* data;
    u32 count;
    u8 pad_0xc[0x1c];
};
