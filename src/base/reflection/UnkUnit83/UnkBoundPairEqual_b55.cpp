// Unit 83 (tools/tu_map.py; base::reflection): equality of two (key, float) values that are each
// held either in place or, with kind 2, through pointers (two identical copies).
#include "types.h"

struct UnkBoundKeyFloat {
    union {
        u64 key;
        const u64* key_ref;
    };
    union {
        f32 value;
        const f32* value_ref;
    };
    u32 kind;  // +0x10: 2 when bound by reference

    const u64& Key() const { return kind == 2 ? *key_ref : key; }
    const f32& Value() const { return kind == 2 ? *value_ref : value; }
};

static __attribute__((always_inline)) bool Equal(const UnkBoundKeyFloat* a, const UnkBoundKeyFloat* b) {
    const u64& ka = a->Key();
    const u64& kb = b->Key();
    if (ka != kb)
        return false;
    return a->Value() == b->Value();
}

bool fn_71000de3b0(const UnkBoundKeyFloat* a, const UnkBoundKeyFloat* b) {
    return Equal(a, b);
}

bool fn_71000de8e8(const UnkBoundKeyFloat* a, const UnkBoundKeyFloat* b) {
    return Equal(a, b);
}
