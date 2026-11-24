// CLightManager's unit: finds the entry of a chained, linked hash map keyed by file-path id, adding a new one
// (key copied, value built by fn_7100da09f8, a file-path id in it set to the key) when there is none; returns
// the entry's value. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

char* fn_7100da09f8(char*);

struct UnkLightHashed_e0c0d4 {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

struct UnkLightValue_e0c0d4 {
    u8 pad_0x0[0x58];
    CFilePathStrId path;
    u8 pad_0x60[0x30];

    UnkLightValue_e0c0d4() { fn_7100da09f8(reinterpret_cast<char*>(this)); }

    void SetPath(const CFilePathStrId& other) {
        if (path.data != other.data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++d->ref_count;
            if (path.data != &pool->empty) {
                --path.data->ref_count;
                if (path.data->ref_count == 0)
                    StringPool_Release(pool, path.data);
            }
            path.data = d;
        }
    }
};

struct UnkLightNode_e0c0d4 {
    CFilePathStrId key;
    UnkLightValue_e0c0d4 value;  // 0x8
    UnkLightNode_e0c0d4* next;   // 0x98
    UnkLightNode_e0c0d4* prev;   // 0xa0
    UnkLightNode_e0c0d4* chain;  // 0xa8

    UnkLightNode_e0c0d4(const CFilePathStrId& k) : key(k), next(nullptr), prev(nullptr), chain(nullptr) {}
};

struct UnkLightMap_e0c0d4 {
    u8 pad_0x0[0x148];
    base::IAllocator* allocator;
    u32 flags;
    u32 count;
    UnkLightNode_e0c0d4** buckets;
    u32 bucket_count;
    u32 divisor;
    u8 pad_0x168[0x18];
    UnkLightNode_e0c0d4* tail;
    UnkLightNode_e0c0d4* head;
};

UnkLightValue_e0c0d4* fn_7100e0c0d4(UnkLightMap_e0c0d4* map, const CFilePathStrId* key) {
    UnkLightNode_e0c0d4** bucket = nullptr;
    if (map->bucket_count != 0) {
        const UnkLightHashed_e0c0d4* h = reinterpret_cast<const UnkLightHashed_e0c0d4*>(key->data);
        u32 index = (h->hash_lo ^ h->hash_hi) % map->divisor;
        bucket = &map->buckets[index];
        if (bucket) {
            for (UnkLightNode_e0c0d4* n = *bucket; n; n = n->chain)
                if (n->key.data == key->data)
                    return &n->value;
        }
    }
    UnkLightNode_e0c0d4* old = *bucket;
    auto* node = static_cast<UnkLightNode_e0c0d4*>(map->allocator->vf_0x40(sizeof(UnkLightNode_e0c0d4), 8, map->flags));
    if (node) {
        new (node) UnkLightNode_e0c0d4(*key);
    }
    node->chain = old;
    *bucket = node;
    if (!map->head)
        map->head = node;
    else if (map->tail) {
        node->prev = map->tail;
        map->tail->next = node;
    }
    ++map->count;
    map->tail = node;
    node->value.SetPath(*key);
    return &node->value;
}
