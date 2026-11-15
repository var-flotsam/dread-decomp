// Unit 23: appends a (key, 0x40-byte value) pair to a keyed vector, growing it when full, and returns
// the last value. Names and layouts are placeholders.
#include "types.h"

typedef u32 __attribute__((may_alias)) UnkAliasU32_l015b6c;
typedef u64 __attribute__((may_alias)) UnkAliasU64_l015b6c;

struct UnkKeyedValue_l015b6c {
    UnkAliasU32_l015b6c w[16];
    UnkKeyedValue_l015b6c& operator=(const UnkKeyedValue_l015b6c& o) {
        for (int i = 0; i < 16; ++i)
            w[i] = o.w[i];
        return *this;
    }
};

struct UnkKeyedVector_l015b6c {
    UnkKeyedValue_l015b6c* values;  // 0x0
    UnkAliasU64_l015b6c* keys;      // 0x8
    u32 count;                      // 0x10
    u32 capacity;                   // 0x14
    u32 step;                       // 0x18
};

void fn_7100015378(UnkKeyedVector_l015b6c* self, u32 capacity);

UnkKeyedValue_l015b6c* fn_7100015b6c(UnkKeyedVector_l015b6c* self, const UnkAliasU64_l015b6c* key,
                                     const UnkKeyedValue_l015b6c* value) {
    if (self->count == self->capacity)
        fn_7100015378(self, self->step + self->count);
    self->keys[self->count] = *key;
    self->values[self->count] = *value;
    self->count += 1;
    UnkKeyedValue_l015b6c* end = &self->values[self->count];
    return self->count ? end - 1 : end;
}
