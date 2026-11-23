#pragma once

#include "types.h"

// The element and array types of src/game/GUI/CMissionLog/fn_7100b5bfdc.cpp (same names, same
// layouts), so the callees keep that file's signatures.
// The element and array types the other files declare these functions with (same names, same layouts).
// The same for 0x78-byte elements in another unit, through its own reserve and push-back.
struct UnkElem78 {
    u8 unk_0x0[0x78];
};
