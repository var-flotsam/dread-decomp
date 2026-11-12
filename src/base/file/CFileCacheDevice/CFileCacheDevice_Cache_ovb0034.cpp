// base::file::CFileCacheDevice unit (tools/tu_map.py, the name from reflection): two pieces of the
// file cache. fn_71000861bc erases a key from the cache's chained hash set (buckets through
// fn_7100086810, the erase at an iterator is fn_71000866dc); fn_7100087124 opens a file through the
// wrapped device (+0x10, its slot 0x40) and records it in the size cache at +0x18 (fn_7100085688),
// reporting a failed open to the registry g_unk_7101d4b5a0. Names and layouts are placeholders.
#include <new>

#include "base/UnkRegistry.h"
#include "base/UnkVector.h"
#include "base/animation/CAnimationResource.h"
#include "base/file/CFileCacheDevice.h"
#include "base/global/UnkStringPool.h"
#include "game/UnkTU63.h"
#include "types.h"

using base::global::UnkPoolIter;

// The set's key: a 64-bit hash, bucketed by its halves xor-ed.
struct UnkCacheKey_ovb0034 {
    u32 lo;
    u32 hi;
};

struct UnkCacheNode_ovb0034 {
    u64 key;
    void* value;
    UnkCacheNode_ovb0034* next;
};

// The set (the layout of base/file/UnkFileCacheSet.h): bucket array, count and modulus, first
// non-empty bucket and node, the nodes' allocator and the modification count at +0x48.
struct UnkCacheSet_ovb0034 {
    UnkCacheNode_ovb0034** buckets;
    u32 count;
    u32 modulus;
    u8 pad_0x10[0x18];
    u32 first;
    u32 pad_0x2c;
    UnkCacheNode_ovb0034* first_node;
    base::UnkAllocator* allocator;
    u32 alloc_flags;
    u32 size;
    u32 version;
};

void fn_71000866dc(char* set, UnkPoolIter* it);

static inline UnkCacheNode_ovb0034** Bucket(UnkCacheSet_ovb0034* set, u32 bucket) {
    return reinterpret_cast<UnkCacheNode_ovb0034**>(fn_7100086810(reinterpret_cast<char*>(set), bucket));
}

// Where a lookup ended: the node (or null), the node before it, the set, the bucket and the set's
// modification count (the layout of base::global::UnkPoolIter).
struct UnkCacheFound_ovb0034 {
    UnkCacheNode_ovb0034* node;
    UnkCacheNode_ovb0034* prev;
    UnkCacheSet_ovb0034* set;
    u32 bucket;
    u32 version;

    UnkCacheFound_ovb0034(UnkCacheNode_ovb0034* n, UnkCacheNode_ovb0034* p, UnkCacheSet_ovb0034* s, u32 b)
        : node(n), prev(p), set(s), bucket(b), version(s ? s->version : 0) {}
};

// Inlined: the lookup, built in place. The out-of-range case comes first, as the original lays
// out its blocks.
static inline void Find(UnkCacheFound_ovb0034* it, UnkCacheSet_ovb0034* set, const UnkCacheKey_ovb0034* key) {
    u32 n = set->count;
    u32 bucket;
    if (!n || (bucket = (key->lo ^ key->hi) % set->modulus) >= n) {
        new (it) UnkCacheFound_ovb0034(nullptr, nullptr, set, -1);
        return;
    }
    UnkCacheNode_ovb0034* prev = nullptr;
    UnkCacheNode_ovb0034* node = *Bucket(set, bucket);
    while (node && node->key != *reinterpret_cast<const u64*>(key)) {
        prev = node;
        node = node->next;
    }
    new (it) UnkCacheFound_ovb0034(node, prev, set, bucket);
}

void fn_71000861bc(UnkCacheSet_ovb0034* set, const UnkCacheKey_ovb0034* key) {
    alignas(UnkCacheFound_ovb0034) char raw[sizeof(UnkCacheFound_ovb0034)];
    UnkCacheFound_ovb0034* it = reinterpret_cast<UnkCacheFound_ovb0034*>(raw);
    Find(it, set, key);
    fn_71000866dc(reinterpret_cast<char*>(set), reinterpret_cast<UnkPoolIter*>(it));
}

// The file being opened: its path's pool entry keeps the path's 64-bit hash at +0x30.
struct UnkCachePathData_ovb0034 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkCacheFile_ovb0034 {
    UnkCachePathData_ovb0034* path;
};

struct UnkCacheHandle_ovb0034 {
    virtual void vf_0x0();
    virtual void Release();
};

struct UnkCachedDevice_ovb0034 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual u32 Open(UnkCacheFile_ovb0034* file, UnkCacheHandle_ovb0034** handle, bool mode, void* allocator,
                     u32 arg, u32 flags);
};

struct UnkCacheSizes_ovb0034 {
    bool enabled;
    u8 rest[0x97];
};

struct UnkFileCacheDevice_ovb0034 {
    u8 pad_0x0[0x10];
    UnkCachedDevice_ovb0034* device;
    UnkCacheSizes_ovb0034 sizes;  // +0x18
};

bool fn_7100085688(UnkCacheSizes_ovb0034* sizes, UnkCacheHandle_ovb0034* handle);

// Status 1 is a plain miss; any other non-zero status is reported to the registry, created on
// first use.
bool fn_7100087124(UnkFileCacheDevice_ovb0034* self, UnkCacheFile_ovb0034* file) {
    UnkCacheHandle_ovb0034* handle = nullptr;
    u32 status = self->device->Open(file, &handle, true, nullptr, 0, 3);
    bool ok = false;
    if (status != 1) {
        if (status == 0) {
            ok = fn_7100085688(&self->sizes, handle);
            if (handle) {
                handle->Release();
                handle = nullptr;
            }
        } else {
            UnkRegistry_0x70* reg = g_unk_7101d4b5a0;
            if (!reg) {
                reg = static_cast<UnkRegistry_0x70*>(fn_71000839a0(0x70, 0x3f));
                fn_7100087388(reg);
                g_unk_7101d4b5a0 = reg;
            }
            fn_7100087dc8(reg, self->device, status);
        }
    }
    return ok;
}
