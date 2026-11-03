// sound::CSoundManager unit (tools/tu_map.py, unit 2646): another copy of the 11-bucket table
// constructor of fn_71002f75ac (src/base/UnkUnit27/UnkBucketTable_Ctor_b41.cpp): a base::UnkVector of
// bucket pointers (step 10, the given flags, the given or default allocator), a word -1, a cleared
// pointer, the allocator again, the flags and the pair (0, 1), then the buckets resized to 11 cleared
// ones. Names and layouts are placeholders.
#include <string.h>

#include "base/UnkVector.h"
#include "types.h"

struct UnkBucket_b42a3;

// base::UnkVector's layout, without its member initialisers.
struct UnkBucketArray_b42a3 {
    UnkBucket_b42a3** data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkBucketTable_b42a3 {
    UnkBucketArray_b42a3 buckets;
    s32 field_0x28;
    void* field_0x30;
    base::UnkAllocator* allocator;
    u32 flags;
    u32 field_0x44;
    u32 field_0x48;

    __attribute__((always_inline)) UnkBucketTable_b42a3(base::UnkAllocator* alloc, u32 fl) {
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
                UnkBucket_b42a3** p = static_cast<UnkBucket_b42a3**>(
                    buckets.allocator->vf_0x40((u64)n * sizeof(UnkBucket_b42a3*), 8, buckets.flags));
                if (UnkBucket_b42a3** old = buckets.data) {
                    u64 j = 0;
                    for (u32 i = 0; i < buckets.count; ++i, ++j)
                        memcpy(&p[j], &old[j], sizeof(UnkBucket_b42a3*));
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

void fn_710102849c(void* self, void* alloc, u32 flags) {
    new (reinterpret_cast<UnkBucketTable_b42a3*>(self))
        UnkBucketTable_b42a3(static_cast<base::UnkAllocator*>(alloc), flags);
}
