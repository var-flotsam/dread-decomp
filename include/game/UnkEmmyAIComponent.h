#pragma once

#include "types.h"

// The E.M.M.I. AI component's base, as its derived components see it: the vtable, then 0x2a6f8 bytes
// in all, where the Proto component's own members start. fn_710053f510 (game/UnkTU811.h) is its
// destructor, which every derived destructor ends in. Name and layout are placeholders.
struct UnkEmmyAIComponent {
    void** vtable;
    u8 pad_0x8[0x2a6f0];
};
