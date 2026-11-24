// Unit 370: constructs a chained hash table in place: its bucket array (an engine vector with growth
// step 10 and the given allocation flags), an empty node list and its allocator, then sizes the
// bucket array to the requested count rounded up to a prime. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/timeline/CEvent.h"
#include "types.h"

struct UnkPrimeBucket_2b2330;

struct UnkPrimeTable_2b2330 {
    UnkPrimeBucket_2b2330** data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
    u32 field_0x28;
    void* field_0x30;
    base::UnkAllocator* node_allocator;
    u32 node_flags;
    u32 field_0x44;
    u32 field_0x48;

    UnkPrimeTable_2b2330(u32 n, u32 fl, base::UnkAllocator* alloc = GetDefaultAllocator())
        : data(nullptr), count(0), capacity(0), step(10), field_0x14(true), flags(fl),
          allocator(alloc ? alloc : GetDefaultAllocator()), field_0x28(0xffffffff), field_0x30(nullptr),
          node_allocator(GetDefaultAllocator()), node_flags(fl), field_0x44(0), field_0x48(1) {
        if (!CEvent_IsPrime(n))
            n = CEvent_GetNextPrimeAtOrAbove(n);
        Resize(n);
    }

    // Grows or shrinks the bucket array to n entries, clearing the new ones.
    void Resize(u32 n) {
        u32 c = count;
        if (c < n) {
            if (capacity < n) {
                UnkPrimeBucket_2b2330** p = static_cast<UnkPrimeBucket_2b2330**>(
                    allocator->vf_0x40((u64)n * sizeof(UnkPrimeBucket_2b2330*), 8, flags));
                if (UnkPrimeBucket_2b2330** old = data) {
                    UnkPrimeBucket_2b2330** dst = p;
                    for (u32 i = 0; i < count; ++i, ++dst, ++old)
                        __builtin_memcpy(dst, old, sizeof(*dst));
                    allocator->vf_0x48(data);
                }
                c = count;
                data = p;
                capacity = n;
            }
            for (u32 i = c; i < n; ++i)
                data[i] = nullptr;
            count = n;
        } else if (c > n) {
            count = n;
        }
    }
};

void fn_71002b2330(void* table, u32 count, u32 size) {
    new (table) UnkPrimeTable_2b2330(count, size);
}
