// Unit 23 (data/tu_map.csv, no reliable name): shrinks a keyed vector (0x40-byte values, 8-byte keys) to
// exactly its count, reallocating both arrays. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

typedef u32 __attribute__((may_alias)) UnkAliasU32_b0252;
typedef u64 __attribute__((may_alias)) UnkAliasU64_b0252;

struct UnkKeyedValue_b0252 {
    UnkAliasU32_b0252 w[16];
    UnkKeyedValue_b0252& operator=(const UnkKeyedValue_b0252& o) {
        for (int i = 0; i < 16; ++i)
            w[i] = o.w[i];
        return *this;
    }
};

struct UnkKeyedVector_b0252 {
    UnkKeyedValue_b0252* values;  // 0x0
    UnkAliasU64_b0252* keys;      // 0x8
    u32 count;                    // 0x10
    u32 capacity;                 // 0x14
    u32 step;                     // 0x18
    u32 flags;                    // 0x1c
    base::UnkAllocator* allocator;  // 0x20
};

void fn_71000159e4(UnkKeyedVector_b0252* self) {
    if (self->count == self->capacity)
        return;
    UnkKeyedValue_b0252* values = static_cast<UnkKeyedValue_b0252*>(
        self->allocator->vf_0x40((u64)self->count << 6, 8, self->flags));
    UnkAliasU64_b0252* keys = static_cast<UnkAliasU64_b0252*>(self->allocator->vf_0x40((u64)self->count << 3, 8, self->flags));
    if (self->values) {
        UnkKeyedValue_b0252* src = self->values;
        UnkKeyedValue_b0252* dst = values;
        for (u32 i = 0; i < self->count; ++i)
            *dst++ = *src++;
        self->allocator->vf_0x48(self->values);
    }
    if (self->keys) {
        if (self->count) {
            UnkAliasU64_b0252* src = self->keys;
            UnkAliasU64_b0252* dst = keys;
            u32 i = 0;
            do {
                *dst++ = *src++;
                ++i;
            } while (i < self->count);
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->values = values;
    self->keys = keys;
    self->capacity = self->count;
}
