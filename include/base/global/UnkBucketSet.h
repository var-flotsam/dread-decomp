#pragma once

// The chained hash set (0x50 bytes) of the string pool (base/global/UnkStringPoolBody.h), the file
// cache (base/file/UnkFileCacheSet.h), the resource registry (src/base/UnkResRegistry.cpp) and others:
// a bucket array, the first non-empty bucket and its first node, the nodes' allocator, the node count
// and a version that every change bumps (iterators keep a copy and stop once it moves). Most users
// follow it with a block pool the nodes come from (UnkPooledBucketSet). Reflection's
// base::global::CRntPooledDictionary<K, V> may be this form; nothing ties the name to the layout yet,
// so the names are placeholders inferred from the code.

#include "base/UnkCloneHelpers.h"
#include "base/global/CRntVector.h"
#include "types.h"

namespace base::global {

// Node: the set's node type, whose last word is the next node of its bucket.
template <class Node>
struct UnkBucketSet {
    CRntVector<Node*> buckets;     // +0: `count` buckets in use; `capacity` (+0xc) is the hash modulus
    u32 first_bucket;              // +0x28: -1 when empty
    Node* first_node;              // +0x30: the first node of first_bucket
    IAllocator* node_allocator;  // +0x38
    u32 node_flags;                // +0x40: passed to node_allocator
    u32 node_count;                // +0x44
    u32 version;                   // +0x48: bumped by each insertion and removal
};
static_assert(sizeof(UnkBucketSet<void>) == 0x50, "UnkBucketSet layout");

// The set followed by the block pool its nodes come from; once set up, node_allocator points at `nodes`.
template <class Node>
struct UnkPooledBucketSet : UnkBucketSet<Node> {
    UnkBlockPool nodes;  // +0x50
};
static_assert(sizeof(UnkPooledBucketSet<void>) == 0x90, "UnkPooledBucketSet layout");

}  // namespace base::global
