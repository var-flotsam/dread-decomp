#pragma once

#include "game_s2/UnkWeakHandle.h"

// The same setter for weak references at other offsets, in other units.
template <u32 Offset>
struct UnkWeakRefAt {
    u8 pad_0x0[Offset];
    UnkWeakHandle* ref;
};
