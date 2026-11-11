#include "base/UnkVector.h"
#include "types.h"

// src/base/global/timeline/CEvent/UnkPrime.cpp.
bool fn_71000669a0(u32 n);
u32 fn_7100066930(u32 n);

struct UnkResBucket_b257j;

// The resource registry's chained set (the path map at +0x10 of the registry, src/base/UnkResRegistry.cpp):
// buckets as an UnkVector, then the first bucket and node in use, a node allocator and counts.
struct UnkResSetInit_b257j {
    UnkResBucket_b257j** data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
    u32 first_bucket;  // 0x28
    void* first_node;  // 0x30
    base::UnkAllocator* node_allocator;  // 0x38
    u32 node_flags;  // 0x40
    u32 node_count;  // 0x44
    u32 version;  // 0x48
};

void fn_71000e9d60(void* set, u32 buckets, u32 flags) {
    UnkResSetInit_b257j* self = static_cast<UnkResSetInit_b257j*>(set);
    base::UnkAllocator* allocator = fn_7100083990();
    self->step = 10;
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->field_0x14 = true;
    self->flags = flags;
    if (!allocator)
        allocator = fn_7100083990();
    self->allocator = allocator;
    self->first_node = nullptr;
    self->first_bucket = -1;
    self->node_allocator = fn_7100083990();
    self->node_flags = flags;
    self->node_count = 0;
    self->version = 1;
    u32 n = buckets;
    if (!fn_71000669a0(n))
        n = fn_7100066930(n);
    u32 c = self->count;
    if (c < n) {
        UnkResBucket_b257j** p;
        if (self->capacity < n) {
            p = static_cast<UnkResBucket_b257j**>(self->allocator->vf_0x40((u64)n * 8, 8, self->flags));
            if (UnkResBucket_b257j** old = self->data) {
                if (self->count) {
                    u32 i = 0;
                    do {
                        __builtin_memcpy(&p[i], &old[i], 8);
                        ++i;
                    } while (i < self->count);
                }
                self->allocator->vf_0x48(self->data);
            }
            c = self->count;
            self->data = p;
            self->capacity = n;
        }
        for (u32 i = c; i < n; ++i)
            __builtin_memset(&self->data[i], 0, 8);
        self->count = n;
    } else if (c > n) {
        self->count = n;
    }
}
