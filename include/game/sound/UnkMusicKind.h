#pragma once

// What a music request does to a slot (the music manager's unit, tools/tu_map.py unit 2637), passed by value:
// its copy constructor makes a caller pass the address of a copy, as the original's callers do. A bit per
// kind where a caller builds it from a stack mode (1 << mode). Names are placeholders.
#include "types.h"

struct UnkKind {
    u32 value;
    UnkKind(u32 v) : value(v) {}
    UnkKind(const UnkKind& o) : value(o.value) {}
};
