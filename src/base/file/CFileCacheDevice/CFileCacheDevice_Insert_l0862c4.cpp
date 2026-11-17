// base::file::CFileCacheDevice unit: creates a cache entry for a key and enters it at the head of its bucket
// in the chained hash set; returns the entry. Names and layouts are placeholders.
#include "base/file/CFileCacheDevice.h"
#include "base/global/CRntVector.h"
#include "base/global/UnkBucketSet.h"
#include "types.h"

extern char g_unk_7101a1ae78[];

struct UnkCacheKey_l0862c4 {
    u32 lo;
    u32 hi;
};

struct UnkCacheEntry_l0862c4 {
    void* vtable;
    u32 flag;
    u32 pad_0xc;
    void* owner;
    u8 loaded;
};

struct UnkCacheNode_l0862c4 {
    UnkCacheKey_l0862c4 key;
    UnkCacheEntry_l0862c4* value;
    UnkCacheNode_l0862c4* next;
};

using UnkCacheSet_l0862c4 = base::global::UnkBucketSet<UnkCacheNode_l0862c4>;

UnkCacheEntry_l0862c4* fn_71000862c4(UnkCacheSet_l0862c4* set, const UnkCacheKey_l0862c4* key, void* owner, u32 flags) {
    auto* entry = static_cast<UnkCacheEntry_l0862c4*>(fn_71000839a0(0x20, 3));
    entry->flag = 0;
    entry->vtable = g_unk_7101a1ae78 + 0x10;
    entry->owner = owner;
    entry->loaded = 0;
    if (flags & 1)
        entry->flag = 1;
    set->version++;
    auto* node = static_cast<UnkCacheNode_l0862c4*>(set->node_allocator->vf_0x40(0x18, 8, set->node_flags));
    if (node) {
        node->key = *key;
        node->value = entry;
        node->next = nullptr;
    }
    u32 index = set->buckets.count ? (key->lo ^ key->hi) % set->buckets.capacity : 0xffffffffu;
    u32 lowest = set->first_bucket;
    set->first_bucket = index < lowest ? index : lowest;
    if (index <= lowest)
        set->first_node = node;
    auto** head = reinterpret_cast<UnkCacheNode_l0862c4**>(fn_7100086810(reinterpret_cast<char*>(set), index));
    node->next = *head;
    *head = node;
    set->node_count++;
    return entry;
}
