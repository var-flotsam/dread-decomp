#pragma once

#include "types.h"

// Copied word by word (as src/game_s7/fn_7100121f78.cpp's UnkValues3).
struct UnkBombValue_b49 {
    u32 values[3];

    UnkBombValue_b49(const UnkBombValue_b49& other) {
        values[0] = other.values[0];
        values[1] = other.values[1];
        values[2] = other.values[2];
    }
};
