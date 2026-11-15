// CActorComponent's unit: compares two flat maps (u64 values at +0, u64 keys at +8, count at +0x10):
// equal when every key of the first is in the second with the same value. Names and layouts are placeholders.
#include "types.h"

struct UnkActorU64Map {
    u64* values;
    u64* keys;
    u32 count;

    u64 FindIndex(u64 key) const {
        u64 i = 0;
        for (; i < count; ++i)
            if (keys[i] == key)
                break;
        return i;
    }
};

bool fn_7100251af0(const UnkActorU64Map* self, const UnkActorU64Map* other) {
    if (self->count != other->count)
        return false;
    for (u32 i = 0; i < self->count; ++i) {
        u64 j = other->FindIndex(self->keys[i]);
        if (j == other->count)
            return false;
        if (other->values[j] != self->values[i])
            return false;
    }
    return true;
}
