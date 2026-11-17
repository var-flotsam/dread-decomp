// Moves of cached iterator values with string-id keys and 8-byte elements, as fn_7100251208
// (src/game_s1/fn_71000cd580.cpp): fn_7100d6e344.
#include "base/UnkCollectionIter.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

__attribute__((flatten)) void fn_7100d6e344(void* dst, const void* src) {
    base::refl_IterValueMove<u64, UnkMovedStrId>(dst, src);
}
