// Prepares an entry (a count-prefixed list of up to 16 pairs of 32-bit values and one 64-bit
// value) and appends a copy of it to the array at +0x317a9e0, growing the array as
// UnkVector::EmplaceBack does but with copy construction.
#include "base/UnkVector.h"
#include "types.h"

struct UnkPair_71001cdba4 {
    u32 first;
    u32 second;
};

struct UnkEntry_71001cdba4 {
    u32 count;
    UnkPair_71001cdba4 items[16];  // 0x4
    u64 field_0x88;

    UnkEntry_71001cdba4(const UnkEntry_71001cdba4& other) {
        __builtin_memcpy(items, other.items, (u64)other.count * sizeof(UnkPair_71001cdba4));
        count = other.count;
        field_0x88 = other.field_0x88;
    }
    ~UnkEntry_71001cdba4() { count = 0; }
};
static_assert(sizeof(UnkEntry_71001cdba4) == 0x90, "entry size");

struct UnkOwner_71001cdba4 {
    u8 pad_0x0[0x317a9e0];
    base::UnkVector<UnkEntry_71001cdba4> entries;
};

void fn_710015c2d0(UnkEntry_71001cdba4* entry);

void fn_71001cdba4(UnkOwner_71001cdba4* self, UnkEntry_71001cdba4* entry) {
    base::UnkVector<UnkEntry_71001cdba4>& v = self->entries;
    fn_710015c2d0(entry);
    u32 n = v.count;
    u32 old_capacity = v.capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && v.step != 0)
        new_capacity = (n / v.step + 1) * v.step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    if (old_capacity < new_capacity) {
        UnkEntry_71001cdba4* p = static_cast<UnkEntry_71001cdba4*>(
            v.allocator->vf_0x40((u64)new_capacity * sizeof(UnkEntry_71001cdba4), 8, v.flags));
        if (v.data) {
            UnkEntry_71001cdba4* src = v.data;
            UnkEntry_71001cdba4* dst = p;
            for (u32 i = 0; i < v.count; ++i, ++src, ++dst) {
                new (dst) UnkEntry_71001cdba4(*src);
                src->~UnkEntry_71001cdba4();
            }
            v.allocator->vf_0x48(v.data);
        }
        v.data = p;
        v.capacity = new_capacity;
        n = v.count;
    }
    new (&v.data[n]) UnkEntry_71001cdba4(*entry);
    ++v.count;
}
