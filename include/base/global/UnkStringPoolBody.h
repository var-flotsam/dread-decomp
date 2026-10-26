#pragma once

// The string pool's own layout (0xe8 bytes). g_unk_7101d47108 and g_unk_7101d47200 point at it, and
// it is what base::global::UnkStringPoolHolder (base/global/CStrId.h) is: the holder's constructor
// builds the pool in place and its destructor tears it down. The code that has the pool as a
// pointer to bytes (src/base/global/fn_71000801e4.cpp, fn_71000803a4.cpp, ...) reaches the same
// fields by offset; the layouts in base/global/UnkStringPool.h are the placeholders for them. Names
// are placeholders inferred from the code.

#include "base/UnkBlockFreeList.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

namespace base::global {

// What a string id points at (UnkStringData in base/global/CStrId.h shows its first fields).
struct UnkStringEntry {
    char* heap_copy;     // +0: the pool's copy of the string, when it keeps one
    s32 ref_count;       // +8
    CRntString buffer;   // +0x10: the string, borrowed or over heap_copy; its key is the hash
    struct {
        bool copy;       // +0x40: the pool keeps its own copy of the string
        u32 field_0x44;
    } tail;              // copied as one word
};
static_assert(sizeof(UnkStringEntry) == 0x48, "UnkStringEntry layout");

// A node of the pool's chained hash set: the hash, the entry and the next node of the bucket.
struct UnkPoolNode {
    u64 key;
    UnkStringEntry data;
    UnkPoolNode* next;
};
static_assert(sizeof(UnkPoolNode) == 0x58, "UnkPoolNode layout");

// The set: the bucket array (an UnkVector; `count` buckets in use of `capacity`, which is also the
// hash modulus), the first non-empty bucket and its first node, the node allocator and the number
// of nodes. `version` counts changes (UnkPoolBucketRef in base/global/UnkStringPool.h).
struct UnkPoolSet {
    UnkVector<char*> buckets;
    u32 first_bucket;  // +0x28: -1 when empty
    UnkPoolNode* first_node;
    UnkAllocator* node_allocator;
    u32 node_flags;
    u32 node_count;
    u32 version;
};
static_assert(sizeof(UnkPoolSet) == 0x50, "UnkPoolSet layout");

// The block pool the set's nodes' memory comes from, laid out as UnkBlockPool
// (base/UnkCloneHelpers.h). It follows the set, and the set's node allocator points at it once
// fn_7100080578 has set the pool up.
struct UnkPoolNodes {
    char* vtable;
    UnkAllocator* parent;  // the allocator the node memory comes from
    UnkBlockFreeList blocks;
    u32 field_0x38;
    bool field_0x3c;
};

// The set with its node pool (what fn_7100080578 takes).
struct UnkPoolSetWithNodes : UnkPoolSet {
    UnkPoolNodes nodes;  // +0x50
};

// What follows the two header fields: the empty string, then the set and the node pool.
struct UnkStringPoolState {
    UnkStringEntry empty;        // +0x10: the empty string, which every default id points at
    UnkPoolSetWithNodes set;     // +0x58
};
static_assert(__builtin_offsetof(UnkStringPoolState, set) == sizeof(UnkStringEntry), "the set follows the empty string");
static_assert(__builtin_offsetof(UnkPoolSetWithNodes, nodes) == sizeof(UnkPoolSet), "the node pool follows the set");

struct UnkStringPoolBody {
    UnkStringPool** slot;  // +0: the global this pool is published in once it's built
    bool field_0x8;
    UnkStringPoolState state;  // +0x10
};
static_assert(sizeof(UnkStringPoolBody) == 0xe8, "UnkStringPoolBody layout");

}  // namespace base::global


// Sets the set up: gives the node pool that follows it room for `nodes` nodes (0x58 bytes each,
// freeing the memory it had) and points the set's node allocator at it, then makes the bucket
// array `buckets` long (growing it, the new buckets empty, or just cutting the count).
void fn_7100080578(base::global::UnkPoolSetWithNodes* set, u32 buckets, u32 nodes);
void fn_71000800dc(char* set);                // frees the set's bucket array
