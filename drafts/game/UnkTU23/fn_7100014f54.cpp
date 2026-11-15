// Unit 23: copy-assigns a keyed vector (0x40-byte values, 8-byte keys): resizes to the source's count,
// then copies values and keys. Names and layouts are placeholders.
#include "types.h"

typedef u32 __attribute__((may_alias)) UnkAliasU32_l014f54;
typedef u64 __attribute__((may_alias)) UnkAliasU64_l014f54;

struct UnkKeyedValue_l014f54 {
    UnkAliasU32_l014f54 w[16];
    UnkKeyedValue_l014f54& operator=(const UnkKeyedValue_l014f54& o) {
        for (int i = 0; i < 16; ++i)
            w[i] = o.w[i];
        return *this;
    }
};

struct UnkKeyedVector_l014f54 {
    UnkKeyedValue_l014f54* values;  // 0x0
    UnkAliasU64_l014f54* keys;      // 0x8
    u32 count;                      // 0x10
    u32 capacity;                   // 0x14
};

void fn_71000159e4(UnkKeyedVector_l014f54* self);
void fn_7100015378(UnkKeyedVector_l014f54* self, u32 capacity);

UnkKeyedVector_l014f54* fn_7100014f54(UnkKeyedVector_l014f54* self, UnkKeyedVector_l014f54* other) {
    if (self == other)
        return self;
    self->count = 0;
    if (other->count != self->capacity) {
        fn_71000159e4(self);
        fn_7100015378(self, other->count);
    }
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
    {
        UnkKeyedValue_l014f54* src = &other->values[i];
        UnkKeyedValue_l014f54* dst = &self->values[i];
        *dst = *src;
    }
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
        self->keys[i] = other->keys[i];
    return self;
}
