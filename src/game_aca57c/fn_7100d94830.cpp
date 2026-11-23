// Erase by key from five hash maps keyed by object pointers (hashed by the two words at +0x30 of
// the object): find the node and its predecessor, then hand the iterator to the map's erase.
// Each map type has its own bucket getter and erase.
#include "base/global/UnkBucketSet.h"
#include "game/UnkHashKeyObj.h"
#include "game/UnkHashNode.h"
#include "types.h"

using UnkHashMap = base::global::UnkBucketSet<UnkHashNode>;

struct UnkHashIter {
    UnkHashNode* node;
    UnkHashNode* prev;
    UnkHashMap* map;
    s32 bucket;
    u32 stamp;
};

static inline UnkHashIter MakeIter(UnkHashNode* node, UnkHashNode* prev, UnkHashMap* map, s32 bucket) {
    UnkHashIter it;
    it.node = node;
    it.prev = prev;
    it.map = map;
    it.bucket = bucket;
    it.stamp = map ? map->version : 0;
    return it;
}

template <UnkHashNode** (*Bucket)(UnkHashMap*, u32)>
__attribute__((always_inline)) static inline UnkHashIter Find(UnkHashMap* map, UnkHashKeyObj* const* key) {
    u32 b;
    if (map->buckets.count == 0 ||
        (b = ((*key)->field_0x30 ^ (*key)->field_0x34) % map->buckets.capacity) >= map->buckets.count)
        return MakeIter(nullptr, nullptr, map, -1);
    UnkHashNode* prev = nullptr;
    for (UnkHashNode* n = *Bucket(map, b); n; prev = n, n = n->next) {
        if (n->key == *key)
            return MakeIter(n, prev, map, b);
    }
    return MakeIter(nullptr, prev, map, b);
}

template <UnkHashNode** (*Bucket)(UnkHashMap*, u32), void (*Erase)(UnkHashMap*, UnkHashIter*)>
__attribute__((always_inline)) static inline void EraseKey(UnkHashMap* map, UnkHashKeyObj* const* key) {
    UnkHashIter it = Find<Bucket>(map, key);
    Erase(map, &it);
}

#define ERASE_KEY(addr, bucket, erase)                                                         \
    UnkHashNode** fn_##bucket(UnkHashMap* map, u32 index);                                     \
    void fn_##erase(UnkHashMap* map, UnkHashIter* it);                                         \
    void fn_##addr(UnkHashMap* map, UnkHashKeyObj* const* key) { EraseKey<fn_##bucket, fn_##erase>(map, key); }

ERASE_KEY(7100d94830, 7100d94654, 7100d94928)
ERASE_KEY(7100d95584, 7100d953a8, 7100d9567c)
ERASE_KEY(7100d962d8, 7100d960fc, 7100d963d0)
ERASE_KEY(7100db34e4, 7100db3308, 7100db35dc)
ERASE_KEY(7100ecb7b4, 7100ecb5d8, 7100ecb8ac)
