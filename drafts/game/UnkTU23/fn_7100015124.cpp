// Unit 23: for every key of a keyed vector, copies the value of the same key from another keyed vector
// when it has one. Names and layouts are placeholders.
#include "types.h"

typedef u32 __attribute__((may_alias)) UnkAliasU32_l015124;
typedef u64 __attribute__((may_alias)) UnkAliasU64_l015124;

struct UnkKeyedValue_l015124 {
    UnkAliasU32_l015124 w[16];
    UnkKeyedValue_l015124& operator=(const UnkKeyedValue_l015124& o) {
        for (int i = 0; i < 16; ++i)
            w[i] = o.w[i];
        return *this;
    }
};

struct UnkKeyedVector_l015124 {
    u8 pad_0x0[8];
    UnkKeyedValue_l015124* values;  // 0x8
    UnkAliasU64_l015124* keys;      // 0x10
    u32 count;                      // 0x18
};

static inline UnkKeyedValue_l015124* Find(UnkKeyedVector_l015124* t, u64 key) {
    for (u32 i = 0; i < t->count; ++i) {
        if (t->keys[i] == key)
            return &t->values[i];
    }
    return &t->values[t->count];
}

static inline bool IsMissing(const UnkKeyedVector_l015124& t, const UnkKeyedValue_l015124* found) {
    return (found == &t.values[t.count]) | (found == nullptr);
}

void fn_7100015124(UnkKeyedVector_l015124* self, UnkKeyedVector_l015124* other) {
    for (u32 i = 0; i < self->count; ++i) {
        UnkKeyedValue_l015124* found = Find(other, self->keys[i]);
        if (IsMissing(*other, found))
            continue;
        self->values[i] = *found;
    }
}
