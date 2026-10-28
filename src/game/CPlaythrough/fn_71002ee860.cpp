// Equality of cached iterator values with 64-bit keys and 8-byte elements (as in fn_71000da828,
// src/game_s1/fn_71000d4d50.cpp): fn_71002ee860, fn_71002eeca8.
#include "base/UnkCollectionIter.h"
#include "types.h"

using base::UnkIterValue;

// A 64-bit key compared through its own operator (a plain u64 index loads it earlier).
struct UnkKey64 {
    u64 value;
    bool operator!=(const UnkKey64& o) const { return value != o.value; }
};
using UnkIterValueU64 = UnkIterValue<u64, UnkKey64>;

bool fn_71002ee860(const UnkIterValueU64& a, const UnkIterValueU64& b) { return a == b; }

bool fn_71002eeca8(const UnkIterValueU64& a, const UnkIterValueU64& b) { return a == b; }
