// CCharClassCollisionComponent's unit: equality of two (key, u32) values that are each held either in place
// or, with kind 2, through pointers (two identical copies); the shape of fn_71000de8e8
// (src/base/reflection/UnkUnit83/UnkBoundPairEqual_b55.cpp) with an integer value. Names are placeholders.
#include "types.h"

namespace {
struct UnkBoundKeyU32_A5_b64 {
    union {
        u64 key;
        const u64* key_ref;
    };
    union {
        u32 value;
        const u32* value_ref;
    };
    u32 kind;  // +0x10: 2 when bound by reference

    const u64& Key() const { return kind == 2 ? *key_ref : key; }
    const u32& Value() const { return kind == 2 ? *value_ref : value; }
};
}  // namespace

static __attribute__((always_inline)) bool Equal_A5_b64(const void* pa, const void* pb) {
    const UnkBoundKeyU32_A5_b64* a = static_cast<const UnkBoundKeyU32_A5_b64*>(pa);
    const UnkBoundKeyU32_A5_b64* b = static_cast<const UnkBoundKeyU32_A5_b64*>(pb);
    const u64& ka = a->Key();
    const u64& kb = b->Key();
    if (ka != kb)
        return false;
    return a->Value() == b->Value();
}

bool fn_710079ba40(const void* a, const void* b) {
    return Equal_A5_b64(a, b);
}

bool fn_710079bf60(const void* a, const void* b) {
    return Equal_A5_b64(a, b);
}
