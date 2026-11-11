// Unit 83 (base::reflection): collection slot 0x58 (equality, fn_71000d7d78) of a reflected keyed array
// (0x18-byte values at +0, u64 keys at +8, count at +0x10): equal when the counts match and every key
// of the first is in the second with a value equal by fn_71000a1ed4 (src/game_s1/fn_71000d4d50.cpp).
// Names are placeholders.
#include "base/reflection.h"
#include "types.h"

struct UnkCompared_0x18 {
    u64 words[3];
};

struct UnkKeyedValues_b61 {
    UnkCompared_0x18* values;
    u64* keys;
    u32 count;

    UnkCompared_0x18* Find(const u64& key) const {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return values + count;
    }
};

bool fn_71000d7d78(const UnkKeyedValues_b61* a, const UnkKeyedValues_b61* b) {
    if (a->count != b->count)
        return false;
    for (u64 i = 0; i < a->count; ++i) {
        UnkCompared_0x18* v = b->Find(a->keys[i]);
        if (v == b->values + b->count)
            return false;
        if (!fn_71000a1ed4(v, &a->values[i]))
            return false;
    }
    return true;
}
