// tools/tu_map.py unit 25 (no class hint): equality tests (reflection slot 0x58) of two 0x20-byte
// reflected values that hold a u32 and a 16-byte key either in place or, when the word at +0x18 is
// 2, through two pointers. The u32s are compared, then the keys as in
// src/base/UnkUnit25/UnkUnit25_PairEquals.cpp. fn_710001d24c and fn_710001d8c4 are the same code
// for two types. Names are placeholders.
#include "types.h"

namespace {

struct UnkKeyPair_b58 {
    u64 first;
    u64 second;
};

struct UnkRefPair_b58 {
    union {
        struct {
            u32 value;
            u32 pad;
            UnkKeyPair_b58 key;
        } in_place;
        struct {
            u32* value;
            UnkKeyPair_b58* key;
        } by_ref;
    };
    s32 mode;

    const u32& Value() const { return mode == 2 ? *by_ref.value : in_place.value; }
    const UnkKeyPair_b58& Key() const { return mode == 2 ? *by_ref.key : in_place.key; }
};

__attribute__((always_inline)) inline bool Equals(const void* pa, const void* pb) {
    auto* a = static_cast<const UnkRefPair_b58*>(pa);
    auto* b = static_cast<const UnkRefPair_b58*>(pb);
    if (a->Value() != b->Value())
        return false;
    const UnkKeyPair_b58& ka = a->Key();
    const UnkKeyPair_b58& kb = b->Key();
    if (__builtin_expect(ka.first != kb.first, 1))
        return false;
    return ka.second == kb.second;
}

}  // namespace

bool fn_710001d24c(const void* a, const void* b) {
    return Equals(a, b);
}

bool fn_710001d8c4(const void* a, const void* b) {
    return Equals(a, b);
}
