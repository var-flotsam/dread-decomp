#pragma once

#include "types.h"

// A position returned in memory (not a plain vector: the original builds it through x8).
struct UnkSluggerPos {
    f32 x;
    f32 y;
    f32 z;
    UnkSluggerPos(const UnkSluggerPos& other);
};
