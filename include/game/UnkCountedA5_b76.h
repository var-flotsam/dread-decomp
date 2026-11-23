#pragma once

#include "game/UnkTU437.h"

// ---- Handle sources behind a lazily resolved handle (state 1 becomes 2): equality of the iterator values
// of two arrays of them, and the copy assignment of one ----
// As src/game/CAccessPointCommanderComponent/CAccessPointCommanderComponent_Handle_ovb0085.cpp declares them.
struct UnkCountedA5_b76 : base::UnkRefCounted {
    union {
        void* field_0x10;
        u64 raw_0x10;
    };
};
