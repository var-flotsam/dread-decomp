#pragma once

#include "types.h"

// A mode word passed by address (the word after it is not set).
struct UnkMusicMode {
    u32 kind;
    u32 field_0x4;
};
