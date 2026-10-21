// Constructors of a hash map with 11 buckets: a bucket array of node pointers (an engine array
// with growth step 10) and the node allocator and flags, both taking the given allocator or the
// default one. The buckets are resized to 11 empty ones (fn_7100d93e5c and its copies).
#include "base/UnkVector.h"
#include "types.h"

using base::UnkAllocator;

struct UnkHashNode;

struct UnkHashBuckets {
    UnkHashNode** data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    UnkAllocator* allocator;

    __attribute__((always_inline)) UnkHashBuckets(UnkAllocator* a, u32 flags)
        : data(nullptr), count(0), capacity(0), step(10), field_0x14(true), flags(flags),
          allocator(a ? a : fn_7100083990()) {}

    __attribute__((always_inline)) void Resize(u32 n) {
        if (count < n) {
            if (capacity < n) {
                UnkHashNode** p = static_cast<UnkHashNode**>(allocator->vf_0x40((u64)n * 8, 8, flags));
                // Copied bitwise: the original reloads `count` after each element.
                if (UnkHashNode** old = data) {
                    for (u32 i = 0; i < count; ++i)
                        __builtin_memcpy(&p[i], &old[i], sizeof(*p));
                    allocator->vf_0x48(data);
                }
                capacity = n;
                data = p;
            }
            for (u32 i = count; i < n; ++i)
                data[i] = nullptr;
        } else if (count == n) {
            return;
        }
        count = n;
    }
};

struct UnkHashMap {
    UnkHashBuckets buckets;
    s32 field_0x28;
    void* field_0x30;
    UnkAllocator* node_allocator;
    u32 node_flags;
    u32 field_0x44;
    u32 field_0x48;

    __attribute__((always_inline)) UnkHashMap(UnkAllocator* a, u32 flags)
        : buckets(a, flags), field_0x28(-1), field_0x30(nullptr), node_allocator(a ? a : fn_7100083990()),
          node_flags(flags), field_0x44(0), field_0x48(1) {
        buckets.Resize(11);
    }
};

// NON_MATCHING(minor): the copy loop compares the 64-bit index instead of its low 32 bits.
void fn_7100d93e5c(UnkHashMap* self, UnkAllocator* a, u32 flags) { new (self) UnkHashMap(a, flags); }
// NON_MATCHING(minor): as fn_7100d93e5c.
void fn_7100d94bb0(UnkHashMap* self, UnkAllocator* a, u32 flags) { new (self) UnkHashMap(a, flags); }
// NON_MATCHING(minor): as fn_7100d93e5c.
void fn_7100d95904(UnkHashMap* self, UnkAllocator* a, u32 flags) { new (self) UnkHashMap(a, flags); }
// NON_MATCHING(minor): as fn_7100d93e5c.
void fn_7100db2fb4(UnkHashMap* self, UnkAllocator* a, u32 flags) { new (self) UnkHashMap(a, flags); }
// NON_MATCHING(minor): as fn_7100d93e5c.
void fn_7100eca5dc(UnkHashMap* self, UnkAllocator* a, u32 flags) { new (self) UnkHashMap(a, flags); }
// NON_MATCHING(minor): as fn_7100d93e5c.
void fn_7100eca778(UnkHashMap* self, UnkAllocator* a, u32 flags) { new (self) UnkHashMap(a, flags); }
