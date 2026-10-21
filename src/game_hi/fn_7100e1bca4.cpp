// Constructs an object holding three chained hash maps (and a zeroed counter after the first),
// each starting with 11 empty buckets.
#include "base/UnkVector.h"
#include "types.h"

struct UnkMapNode;

// Same layout as the chained maps in base/UnkHashMap.h.
struct UnkChainedMap {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    base::UnkVector<UnkMapNode*> buckets;
    UnkMapNode* head;
    UnkMapNode* tail;

    __attribute__((always_inline)) UnkChainedMap() : allocator(fn_7100083990()), flags(0xe), count(0), buckets(0xe), head(nullptr), tail(nullptr) {
        ResizeBuckets(11);
    }

    // Sets the bucket count, clearing new buckets.
    void ResizeBuckets(u32 n) {
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
};

struct UnkMapSet {
    UnkChainedMap map0;
    u32 field_0x48;
    UnkChainedMap map1;
    UnkChainedMap map2;
    __attribute__((always_inline)) UnkMapSet() : field_0x48(0) {}
};

void fn_7100e1bca4(UnkMapSet* self) {
    new (self) UnkMapSet();
}
