// Equality of cached iterator values with 64-bit keys and 8-byte elements (as in fn_71000da828,
// src/game_s1/fn_71000d4d50.cpp): fn_71002ee860, fn_71002eeca8.
#include "base/UnkCollectionIter.h"
#include "game/UnkKey64.h"
#include "types.h"

using base::UnkIterValue;

using UnkIterValueU64 = UnkIterValue<u64, UnkKey64>;

bool fn_71002ee860(const UnkIterValueU64& a, const UnkIterValueU64& b) { return a == b; }

bool fn_71002eeca8(const UnkIterValueU64& a, const UnkIterValueU64& b) { return a == b; }
