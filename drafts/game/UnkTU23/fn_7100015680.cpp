// Unit 23: walks one keyed vector with its own cursor and, for each key that the other keyed vector has,
// copies the 4x4 matrix over, keeping the old last column when the source's w is not 1. Names and
// layouts are placeholders.
#include "types.h"

typedef u32 __attribute__((may_alias)) UnkAliasU32_l015680;
typedef u64 __attribute__((may_alias)) UnkAliasU64_l015680;

struct UnkKeyedValue_l015680 {
    UnkAliasU32_l015680 w[16];
    UnkKeyedValue_l015680& operator=(const UnkKeyedValue_l015680& o) {
        for (int i = 0; i < 16; ++i)
            w[i] = o.w[i];
        return *this;
    }
};

struct UnkKeyedVector_l015680 {
    u8 pad_0x0[8];
    UnkKeyedValue_l015680* values;  // 0x8
    UnkAliasU64_l015680* keys;      // 0x10
    u32 count;                      // 0x18
    u8 pad_0x1c[0x14];
    UnkKeyedValue_l015680* cursor;  // 0x30
};

static inline f32 AsFloat(u32 bits) {
    f32 f;
    __builtin_memcpy(&f, &bits, sizeof(f));
    return f;
}

static inline UnkKeyedValue_l015680* Find(UnkKeyedVector_l015680* t, u64 key) {
    for (u32 i = 0; i < t->count; ++i) {
        if (t->keys[i] == key)
            return &t->values[i];
    }
    return &t->values[t->count];
}

static inline bool IsPresent(const UnkKeyedVector_l015680& t, const UnkKeyedValue_l015680* found) {
    return found != &t.values[t.count] && found != nullptr;
}

void fn_7100015680(UnkKeyedVector_l015680* target, UnkKeyedVector_l015680* source) {
    source->cursor = source->values;
    if (source->count == 0)
        return;
    do {
        UnkKeyedValue_l015680* src = source->cursor;
        u32 index = src - source->values;
        u64 key = source->keys[index];
        source->cursor = src + 1;
        UnkKeyedValue_l015680* dst = Find(target, key);
        if (!IsPresent(*target, dst))
            continue;
        u32 old3 = dst->w[3];
        u32 old7 = dst->w[7];
        u32 old11 = dst->w[11];
        *dst = *src;
        if (AsFloat(src->w[15]) != 1.0f) {
            dst->w[15] = 0x3f800000;
            dst->w[3] = old3;
            dst->w[7] = old7;
            dst->w[11] = old11;
        }
    } while (source->cursor != &source->values[source->count]);
}
