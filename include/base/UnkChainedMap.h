#pragma once

// The chained hash map's header and its 11-bucket start, as base/UnkHashMap.h describes the
// layout: an allocator at +0, the flags word 0xe at +8, the entry count at +0xc, the bucket array
// (an UnkVector with flags 0xe) at +0x10 and the first and last node at +0x38 and +0x40. Placeholder
// names inferred from fn_7100e1bca4 and fn_71011a870c.

#include "base/UnkVector.h"
#include "types.h"

struct UnkMapNode;

struct UnkChainedMap {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    base::UnkVector<UnkMapNode*> buckets;
    UnkMapNode* head;
    UnkMapNode* tail;

    // The map before its buckets exist (some owners size them with their own initialiser).
    __attribute__((always_inline)) UnkChainedMap()
        : allocator(fn_7100083990()), flags(0xe), count(0), buckets(0xe), head(nullptr), tail(nullptr) {}

    // Sets the bucket count, clearing new buckets. The array's pointer is stored after its
    // capacity (the constructors of fn_71011a870c and fn_7100ee1f34); ResizeBucketsPtrFirst has
    // them the other way round (fn_7100fd3804).
    __attribute__((always_inline)) void ResizeBuckets(u32 n) {
        if (buckets.count < n) {
            if (buckets.capacity < n) {
                UnkMapNode** p = static_cast<UnkMapNode**>(buckets.allocator->vf_0x40((u64)n * sizeof(UnkMapNode*), 8, buckets.flags));
                if (buckets.data) {
                    __builtin_memcpy(p, buckets.data, (u64)buckets.count * sizeof(UnkMapNode*));
                    buckets.allocator->vf_0x48(buckets.data);
                }
                buckets.capacity = n;
                buckets.data = p;
            }
            __builtin_memset(&buckets.data[buckets.count], 0, (u64)(n - buckets.count) * sizeof(UnkMapNode*));
            buckets.count = n;
        } else if (n < buckets.count) {
            buckets.count = n;
        }
    }

    __attribute__((always_inline)) void ResizeBucketsPtrFirst(u32 n) {
        if (buckets.count < n) {
            if (buckets.capacity < n) {
                UnkMapNode** p = static_cast<UnkMapNode**>(buckets.allocator->vf_0x40((u64)n * sizeof(UnkMapNode*), 8, buckets.flags));
                if (buckets.data) {
                    __builtin_memcpy(p, buckets.data, (u64)buckets.count * sizeof(UnkMapNode*));
                    buckets.allocator->vf_0x48(buckets.data);
                }
                buckets.data = p;
                buckets.capacity = n;
            }
            __builtin_memset(&buckets.data[buckets.count], 0, (u64)(n - buckets.count) * sizeof(UnkMapNode*));
            buckets.count = n;
        } else if (n < buckets.count) {
            buckets.count = n;
        }
    }
};
static_assert(sizeof(UnkChainedMap) == 0x48, "UnkChainedMap layout");
