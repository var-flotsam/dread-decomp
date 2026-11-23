#pragma once

#include "types.h"

// A plain word, and two words and a value, copied piece by piece.
struct UnkWord {
    u64 value;

    UnkWord(const UnkWord& other) { value = other.value; }
};
