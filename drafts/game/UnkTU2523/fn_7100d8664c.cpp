// UnkTU2523 (CEntity): compares two flat maps (u64 values at +0, u64 keys at +8, count at +0x10) for equality.
// Names and layouts are placeholders.
#include "types.h"

struct UnkEntityU64Map {
    u64* values;
    u64* keys;
    u32 count;

    u32 Find(u64 key) const {
        u64 n = count;
        for (u64 i = 0; i != n; ++i)
            if (keys[i] == key)
                return (u32)i;
        return (u32)n;
    }
};

bool fn_7100d8664c(const UnkEntityU64Map* self, const UnkEntityU64Map* other) {
    if (self->count != other->count)
        return false;
    for (u32 i = 0; i < self->count; ++i) {
        u32 j = other->Find(self->keys[i]);
        if (j == other->count)
            return false;
        if (other->values[j] != self->values[i])
            return false;
    }
    return true;
}
