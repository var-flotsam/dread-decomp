#include "base/global/CRntVector.h"
#include "base/global/UnkBucketSet.h"
#include "types.h"
#include "base/global/timeline/CEvent.h"

struct UnkResBucket_b257j;

// The resource registry's chained set (base/global/UnkBucketSet.h), the path map at +0x10 of the
// registry (src/base/UnkResRegistry.cpp).
using UnkResSetInit_b257j = base::global::UnkBucketSet<UnkResBucket_b257j>;

void fn_71000e9d60(void* set, u32 buckets, u32 flags) {
    UnkResSetInit_b257j* self = static_cast<UnkResSetInit_b257j*>(set);
    base::UnkAllocator* allocator = GetDefaultAllocator();
    self->buckets.step = 10;
    self->buckets.data = nullptr;
    self->buckets.count = 0;
    self->buckets.capacity = 0;
    self->buckets.field_0x14 = true;
    self->buckets.flags = flags;
    if (!allocator)
        allocator = GetDefaultAllocator();
    self->buckets.allocator = allocator;
    self->first_node = nullptr;
    self->first_bucket = -1;
    self->node_allocator = GetDefaultAllocator();
    self->node_flags = flags;
    self->node_count = 0;
    self->version = 1;
    u32 n = buckets;
    if (!CEvent_IsPrime(n))
        n = CEvent_GetNextPrimeAtOrAbove(n);
    u32 c = self->buckets.count;
    if (c < n) {
        UnkResBucket_b257j** p;
        if (self->buckets.capacity < n) {
            p = static_cast<UnkResBucket_b257j**>(self->buckets.allocator->vf_0x40((u64)n * 8, 8, self->buckets.flags));
            if (self->buckets.data) {
                // The buckets are moved over one by one.
                UnkResBucket_b257j** src = self->buckets.data;
                UnkResBucket_b257j** dst = p;
                for (u32 i = 0; i < self->buckets.count; ++i, ++dst, ++src)
                    *dst = *src;
                self->buckets.allocator->vf_0x48(self->buckets.data);
            }
            c = self->buckets.count;
            self->buckets.data = p;
            self->buckets.capacity = n;
        }
        for (u32 i = c; i < n; ++i)
            __builtin_memset(&self->buckets.data[i], 0, 8);
        self->buckets.count = n;
    } else if (c > n) {
        self->buckets.count = n;
    }
}
