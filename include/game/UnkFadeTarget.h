#pragma once

#include "types.h"

// The object whose node at +0x18 shows the fade, and whether it is hidden for the mode holder.
struct UnkFadeTarget {
    u8 unk_0x0[0x18];
    u8 node[0x138];
    // A union member (no type-based alias info): the original reloads the target after storing it.
    union {
        bool hidden;  // +0x150
    };
};
