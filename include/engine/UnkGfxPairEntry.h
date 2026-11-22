#pragma once

#include "types.h"

// The map at +0x3183628 as a lookup sees it: 16-byte entries whose second word it returns, keyed by
// string ids.
struct UnkGfxPairEntry {
    u64 first;
    void* second;
};
