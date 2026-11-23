#pragma once

#include "types.h"

// The player's +0x208: a movement helper.
struct UnkHelper {
    void** vtable;

    bool vf_0x3b8() { return reinterpret_cast<bool (*)(UnkHelper*)>(vtable[0x3b8 / 8])(this); }
};
