// base::file::CFileCacheDevice unit: creates a cache entry for a key and enters it at the head of its bucket
// in the chained hash set; returns the entry. Names and layouts are placeholders.
#include "base/file/CFileCacheDevice.h"
#include "base/global/CRntVector.h"
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

struct UnkNodeAllocator_l0862c4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void* vf_0x40(u64 size, u64 align, u32 flags);
};

struct UnkCacheSet_l0862c4 {
    u8 pad_0x0[8];
    u32 bucket_count;
    u32 modulus;
    u8 pad_0x10[0x18];
    u32 lowest;
    u32 pad_0x2c;
    UnkCacheNode_l0862c4* first;
    UnkNodeAllocator_l0862c4* allocator;
    u32 alloc_flags;
    u32 count;
    u32 version;
};


UnkCacheEntry_l0862c4* fn_71000862c4(UnkCacheSet_l0862c4* set, const UnkCacheKey_l0862c4* key, void* owner, u32 flags) {
    auto* entry = static_cast<UnkCacheEntry_l0862c4*>(fn_71000839a0(0x20, 3));
    entry->flag = 0;
    entry->vtable = g_unk_7101a1ae78 + 0x10;
    entry->owner = owner;
    entry->loaded = 0;
    if (flags & 1)
        entry->flag = 1;
    set->version++;
    auto* node = static_cast<UnkCacheNode_l0862c4*>(set->allocator->vf_0x40(0x18, 8, set->alloc_flags));
    if (node) {
        node->key = *key;
        node->value = entry;
        node->next = nullptr;
    }
    u32 index = set->bucket_count ? (key->lo ^ key->hi) % set->modulus : 0xffffffffu;
    u32 lowest = set->lowest;
    set->lowest = index < lowest ? index : lowest;
    if (index <= lowest)
        set->first = node;
    auto** head = reinterpret_cast<UnkCacheNode_l0862c4**>(fn_7100086810(reinterpret_cast<char*>(set), index));
    node->next = *head;
    *head = node;
    set->count++;
    return entry;
}
