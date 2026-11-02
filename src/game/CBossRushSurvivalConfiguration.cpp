// A lookup in parallel arrays of 4-byte values and 4-byte keys (base::UnkKeyedArray, as
// fn_71002eef6c in src/game/CPlaythrough/fn_71002eef6c.cpp for 8-byte ones), in
// CBossRushSurvivalConfiguration's unit: the value stored for a key, or null.
#include "base/UnkCloneCalls.h"
#include "types.h"

struct UnkKeyedWords_7100dd0858;
using UnkWordsByWordKey = base::UnkKeyedArray<UnkKeyedWords_7100dd0858, u32, u32>;

u32* fn_7100dd0858(UnkWordsByWordKey* a, const u32* key) {
    u32* p = a->Find(*key);
    return p == a->End() ? nullptr : p;
}
