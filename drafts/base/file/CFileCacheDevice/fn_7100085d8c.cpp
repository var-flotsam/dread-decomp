// base::file::CFileCacheDevice unit: clears the file cache's chained hash set: first every entry's owned
// value is destroyed, then the nodes are freed and the buckets emptied. Names and layouts are placeholders.
#include "base/UnkHashMap.h"
#include "base/file/CFileCacheDevice.h"
#include "types.h"

struct UnkCacheValue_l085d8c {
    virtual void vf_0x0();
    virtual void Delete();
};

struct UnkCacheNode_l085d8c {
    u8 pad_0x0[8];
    UnkCacheValue_l085d8c* value;
    UnkCacheNode_l085d8c* next;
};

struct UnkCacheSet_l085d8c {
    UnkCacheNode_l085d8c** buckets;
    u32 bucketCount;
    u8 pad_0xc[0x28 - 0xc];
    u32 bucket;
    UnkCacheNode_l085d8c* node;
    u8 pad_0x38[0x48 - 0x38];
    u32 version;
};

struct UnkCacheCursor_l085d8c {
    UnkCacheNode_l085d8c* node;
    UnkCacheSet_l085d8c* set;
    u32 bucket;
    u32 version;
    UnkCacheCursor_l085d8c(UnkCacheNode_l085d8c* n, UnkCacheSet_l085d8c* s, u32 b)
        : node(n), set(s), bucket(b), version(s ? s->version : 0) {}
    bool Valid() const { return set && set->version == version; }
    void Next() {
        node = node->next;
        if (node)
            return;
        do {
            if (++bucket >= set->bucketCount) {
                node = nullptr;
                return;
            }
            node = set->buckets[bucket];
        } while (!node);
    }
};

struct UnkTag_l085d8c;
template <> struct base::UnkChainedSetTraits<UnkTag_l085d8c> {
    static char** bucket(char* self, u32 i) { return fn_7100086810(self, i); }
};
struct UnkTrivialKey_l085d8c {};

void fn_7100085d8c(UnkCacheSet_l085d8c* set) {
    UnkCacheCursor_l085d8c cur(set->node, set, set->bucket);
    for (; cur.node; cur.Next()) {
        if (cur.node->value) {
            cur.node->value->Delete();
            cur.node->value = nullptr;
        }
        if (!cur.Valid())
            break;
    }
    base::refl_ChainedSetClear<UnkTag_l085d8c, UnkTrivialKey_l085d8c, 0x10>(reinterpret_cast<char*>(set));
}
