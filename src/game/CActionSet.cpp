// CActionSet's unit (unit 1729 of tools/tu_map.py, the name from reflection): the lookup in its hash
// map of actions keyed by two hashes. Names and layouts are placeholders.
#include "types.h"

// A node: the two keys, the value from +0x10 and the next node in the bucket at +0x1e0.
struct UnkActionSetNode {
    u64 key2;
    u64 key1;
    u8 value[0x1d0];
    UnkActionSetNode* next;
};
static_assert(__builtin_offsetof(UnkActionSetNode, next) == 0x1e0, "UnkActionSetNode layout");

struct UnkActionSetMap {
    u8 pad_0x0[0x78];
    UnkActionSetNode** buckets;  // 0x78
    u32 count;                   // 0x80
    u32 bucket_count;            // 0x84
};

// The node holding the pair of keys, or null; the bucket is the keys' sum, its halves xor-ed, modulo
// the bucket count.
static inline UnkActionSetNode* FindNode(UnkActionSetMap* self, const u64* key1, const u64* key2) {
    if (self->count == 0)
        return nullptr;
    u64 sum = *key2 + *key1;
    u32 hash = (u32)(sum >> 32) ^ (u32)sum;
    UnkActionSetNode** bucket = &self->buckets[hash % self->bucket_count];
    if (!bucket)
        return nullptr;
    UnkActionSetNode* node = *bucket;
    while (node && (*key1 != node->key1 || *key2 != node->key2))
        node = node->next;
    return node;
}

// The value stored under the pair of keys, or null.
// NON_MATCHING(minor): scheduling: the original loads the bucket count and the bucket array after
// folding the hash (other registers for the division); the behaviour is the same.
void* fn_7100b8c61c(UnkActionSetMap* self, const u64* key1, const u64* key2) {
    UnkActionSetNode* node = FindNode(self, key1, key2);
    return node ? node->value : nullptr;
}
