// CGameBlackboard unit: equality of a two-level dictionary (key -> dictionary of key -> float).
// Names and layouts are placeholders.
#include "types.h"

struct UnkFloatDict {
    f32* values;
    u64* keys;
    u32 count;
    u32 field_0x14;
    u64 field_0x18;
    u64 field_0x20;

    // Index of key, or count when missing.
    u32 Find(u64 key) const {
        for (u64 i = 0; i != count; i++) {
            if (keys[i] == key)
                return (u32)i;
        }
        return count;
    }
};

struct UnkFloatDictDict {
    UnkFloatDict* values;
    u64* keys;
    u32 count;

    u32 Find(u64 key) const {
        for (u64 i = 0; i != count; i++) {
            if (keys[i] == key)
                return (u32)i;
        }
        return count;
    }
};

static bool FloatDictEquals(const UnkFloatDict& a, const UnkFloatDict& b) {
    if (a.count != b.count)
        return false;
    for (u32 i = 0; i != a.count; i++) {
        u64 j = b.Find(a.keys[i]);
        if (j == b.count)
            return false;
        if (a.values[i] != b.values[j])
            return false;
    }
    return true;
}

bool fn_7100aee42c(const UnkFloatDictDict* a, const UnkFloatDictDict* b) {
    if (a->count != b->count)
        return false;
    for (u32 i = 0; i != a->count; i++) {
        u64 j = b->Find(a->keys[i]);
        if (j == b->count)
            return false;
        if (!FloatDictEquals(a->values[i], b->values[j]))
            return false;
    }
    return true;
}
