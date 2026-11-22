#pragma once

#include "game/UnkTU2553.h"

// A link into the weak handle list of a track: the context and callback it is given, and the handle it holds.
// A link into a track's weak handle list: the context and callback it is given, and the handle it holds.
struct UnkTrackLink_b152 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
    UnkWeakHandle* handle;
};
