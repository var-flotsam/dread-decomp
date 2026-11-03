// Unit 27 (tools/tu_map.py, no name hint) and two others: the constructor of a table of 11 buckets,
// one copy per unit: a base::UnkVector of bucket pointers (step 10, the given flags, the given
// allocator or the default one) followed by a word -1, a cleared pointer, the allocator again (asked
// for a second time when none was given), the flags and the pair (0, 1); the bucket array is then
// resized to 11 cleared buckets (grown element by element; the copy loop counts in 32 bits beside a
// 64-bit index). Names and layouts are placeholders.
#include <string.h>

#include "base/UnkVector.h"
#include "types.h"

struct UnkBucket_b41a1;

// base::UnkVector's layout, without its member initialisers.
struct UnkBucketArray_b41a1 {
    UnkBucket_b41a1** data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkBucketTable_b41a1 {
    UnkBucketArray_b41a1 buckets;
    s32 field_0x28;
    void* field_0x30;
    base::UnkAllocator* allocator;
    u32 flags;
    u32 field_0x44;
    u32 field_0x48;

    __attribute__((always_inline)) UnkBucketTable_b41a1(base::UnkAllocator* alloc, u32 fl) {
        buckets.data = nullptr;
        buckets.count = 0;
        buckets.capacity = 0;
        buckets.step = 10;
        buckets.field_0x14 = true;
        buckets.flags = fl;
        if (alloc) {
            buckets.allocator = alloc;
            field_0x28 = -1;
            field_0x30 = nullptr;
            allocator = alloc;
        } else {
            buckets.allocator = fn_7100083990();
            field_0x28 = -1;
            field_0x30 = nullptr;
            allocator = fn_7100083990();
        }
        flags = fl;
        field_0x44 = 0;
        field_0x48 = 1;
        Resize(11);
    }

    __attribute__((always_inline)) void Resize(u32 n) {
        if (buckets.count < n) {
            if (buckets.capacity < n) {
                UnkBucket_b41a1** p = static_cast<UnkBucket_b41a1**>(
                    buckets.allocator->vf_0x40((u64)n * sizeof(UnkBucket_b41a1*), 8, buckets.flags));
                if (UnkBucket_b41a1** old = buckets.data) {
                    u64 j = 0;
                    for (u32 i = 0; i < buckets.count; ++i, ++j)
                        memcpy(&p[j], &old[j], sizeof(UnkBucket_b41a1*));
                    buckets.allocator->vf_0x48(buckets.data);
                }
                buckets.capacity = n;
                buckets.data = p;
            }
            for (u32 i = buckets.count; i < n; i++)
                buckets.data[i] = nullptr;
            buckets.count = n;
        } else if (n < buckets.count) {
            buckets.count = n;
        }
    }
};

// Signatures as other files already declare them (src/base/UnkResFactoryRegistry.cpp,
// src/game/CMinimapManager.cpp); the table is the first parameter.
#define UNK_BUCKET_TABLE_B41A1(addr, Self, Alloc)                                    \
    void fn_##addr(Self self, Alloc alloc, u32 flags) {                               \
        new (reinterpret_cast<UnkBucketTable_b41a1*>(self))                           \
            UnkBucketTable_b41a1(static_cast<base::UnkAllocator*>(alloc), flags);     \
    }

UNK_BUCKET_TABLE_B41A1(71000204d4, u64*, void*)
UNK_BUCKET_TABLE_B41A1(7100021230, u64*, void*)
UNK_BUCKET_TABLE_B41A1(7100021f84, u64*, void*)
UNK_BUCKET_TABLE_B41A1(71000e98a0, UnkBucketTable_b41a1*, base::UnkAllocator*)
UNK_BUCKET_TABLE_B41A1(71002f75ac, void*, void*)
