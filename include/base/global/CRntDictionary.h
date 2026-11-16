#pragma once

// The engine's dictionary, base::global::CRntDictionary (the name reflection gives it): a chained hash map
// whose nodes are also linked in one list, the list reflection walks. Nodes come from the dictionary's
// allocator; the buckets are a CRntVector of node pointers, indexed by the key's hash modulo their capacity.
// Only what the reflection descriptors use is written; insertion, removal and clearing are still by address.
// Layout as base/UnkHashMap.h describes it.

#include "base/global/CRntVector.h"
#include "types.h"

namespace base::global {

template <class K, class V>
struct CRntDictionary {
    using key_type = K;
    using value_type = V;

    struct TNode {
        K key;
        V value;
        TNode* next;         // the walk's next node
        TNode* unk_link;     // the list's other link (placeholder)
        TNode* bucket_next;  // the next node of the same bucket
    };

    UnkAllocator* allocator;  // the nodes'
    u32 flags;
    u32 count;
    CRntVector<TNode*> buckets;
    TNode* field_0x38;
    TNode* first;  // where the walk starts

    // The key's bucket, or null while there are no buckets.
    TNode* const* BucketFor(const K& key) const {
        if (buckets.count == 0)
            return nullptr;
        u32 index = key.Hash() % buckets.capacity;
        return buckets.data + index;
    }

    // The value under key, or null.
    V* Find(const K& key) const {
        TNode* const* bucket = BucketFor(key);
        if (!bucket)
            return nullptr;
        for (TNode* node = *bucket; node; node = node->bucket_next)
            if (node->key == key)
                return &node->value;
        return nullptr;
    }
};

}  // namespace base::global
