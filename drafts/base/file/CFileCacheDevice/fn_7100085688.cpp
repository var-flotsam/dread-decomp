// base::file::CFileCacheDevice unit (tools/tu_map.py unit 62): loads the size cache from an open
// file. The file holds a count, then that many 12-byte records (a 64-bit key and a 32-bit size),
// each put into the cache's chained hash set; a short or truncated file leaves the cache empty and
// unloaded. Names and layouts are placeholders.
#include "base/UnkHashMap.h"
#include "base/file/UnkFileCacheSet.h"
#include "types.h"

char** fn_7100085afc(char* set, u32 index);
void fn_7100085930(UnkFileCacheSet* self, u32 buckets, u32 nodes);

struct UnkTag_l085688;
template <>
struct base::UnkChainedSetTraits<UnkTag_l085688> {
    static char** bucket(char* self, u32 i) { return fn_7100085afc(self, i); }
};
struct UnkTrivialKey_l085688 {};

// The file the sizes are read from.
struct UnkSizeFile_l085688 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual const u8* Data();
    virtual s64 Size();
};

struct UnkSizeNode_l085688 {
    u64 key;
    u32 size;
    UnkSizeNode_l085688* next;

    UnkSizeNode_l085688(u64 k, u32 s) : key(k), size(s), next(nullptr) {}
};

struct UnkSizeCache_l085688 {
    bool loaded;
    base::global::UnkPooledBucketSet<UnkSizeNode_l085688> set;  // +0x8

    char* SetBytes() { return reinterpret_cast<char*>(&set); }

    void Reset() {
        base::refl_ChainedSetClear<UnkTag_l085688, UnkTrivialKey_l085688, 0x10>(SetBytes());
        loaded = false;
    }

    void SetUp(u32 buckets, u32 nodes) { fn_7100085930(reinterpret_cast<UnkFileCacheSet*>(&set), buckets, nodes); }

    void Insert(u64 key, u32 size) {
        ++set.version;
        void* raw = set.node_allocator->vf_0x40(sizeof(UnkSizeNode_l085688), 8, set.node_flags);
        UnkSizeNode_l085688* node = raw ? new (raw) UnkSizeNode_l085688(key, size) : nullptr;
        u32 bucket = set.buckets.count ? u32(key) % set.buckets.capacity : u32(-1);
        u32 first = set.first_bucket;
        set.first_bucket = bucket < first ? bucket : first;
        if (bucket <= first)
            set.first_node = node;
        UnkSizeNode_l085688** slot = reinterpret_cast<UnkSizeNode_l085688**>(fn_7100085afc(SetBytes(), bucket));
        node->next = *slot;
        *slot = node;
        ++set.node_count;
    }

    bool Read(const u8* data, s64 size) {
        u32 count = size >= 4 ? *reinterpret_cast<const u32*>(data) : 0;
        SetUp(count, count);
        if (size < 4)
            return false;
        const u8* end = data + size;
        for (const u8* p = data + 4; p < end; p += 12) {
            if (p + 8 > end || p + 12 > end)
                return false;
            Insert(*reinterpret_cast<const u64*>(p), *reinterpret_cast<const u32*>(p + 8));
        }
        return true;
    }
};

struct UnkCacheSizes_ovb0034;
struct UnkCacheHandle_ovb0034;

bool fn_7100085688(UnkCacheSizes_ovb0034* sizes, UnkCacheHandle_ovb0034* handle) {
    UnkSizeCache_l085688* self = reinterpret_cast<UnkSizeCache_l085688*>(sizes);
    UnkSizeFile_l085688* file = reinterpret_cast<UnkSizeFile_l085688*>(handle);
    if (self->loaded)
        self->Reset();
    const u8* data = file->Data();
    bool ok = self->Read(data, file->Size());
    if (!ok)
        self->Reset();
    self->loaded = ok;
    return ok;
}
