// CGameBlackboard unit: equality of two reflected values holding an id and a dictionary of id -> float,
// each member inline or (mode 2) through a pointer. Names and layouts are placeholders.
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

struct UnkHolder {
    union {
        u64 value;
        const u64* ptr;
    } id;
    union {
        UnkFloatDict value;
        const UnkFloatDict* ptr;
    } dict;
    u32 mode;
    const u64& Id() const { return mode == 2 ? *id.ptr : id.value; }
    const UnkFloatDict& Dict() const { return mode == 2 ? *dict.ptr : dict.value; }
};

static inline bool Equal(const void* a_, const void* b_) {
    const UnkHolder* a = static_cast<const UnkHolder*>(a_);
    const UnkHolder* b = static_cast<const UnkHolder*>(b_);
    if (a->Id() != b->Id())
        return false;
    const UnkFloatDict& x = a->Dict();
    const UnkFloatDict& y = b->Dict();
    if (x.count != y.count)
        return false;
    for (u64 i = 0; i < x.count; i++) {
        u32 j = y.Find(x.keys[i]);
        if (j == y.count)
            return false;
        if (y.values[j] != x.values[i])
            return false;
    }
    return true;
}

bool fn_7100aed3a8(const void* a, const void* b) {
    return Equal(a, b);
}
