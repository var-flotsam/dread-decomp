#pragma once

#include "game_s2/UnkWeakHandle.h"

// Refers the reference at 0xf0 to `other`'s weak handle.
struct UnkWeakRefF0 {
    u8 unk_0x0[0xf0];
    UnkWeakHandle* field_0xf0;
};
