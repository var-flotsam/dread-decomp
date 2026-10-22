// Copies of fn_7100d94830 (src/game_aca57c/fn_7100d94830.cpp) for other types, with their own callees and data (the names, strings and comments below are the original's): fn_71002f7e04, fn_7100020eb0, fn_7100021c04, fn_7100022958, fn_71000eca70, fn_710102c298, fn_7101195894, fn_7101195b0c, fn_7101195d84.
// Erase by key from five hash maps keyed by object pointers (hashed by the two words at +0x30 of
// the object): find the node and its predecessor, then hand the iterator to the map's erase.
// Each map type has its own bucket getter and erase.
#include "types.h"

struct UnkHashKeyObj {
    u8 unk_0x0[0x30];
    u32 field_0x30;
    u32 field_0x34;
};

struct UnkHashNode {
    UnkHashKeyObj* key;
    u8 unk_0x8[0x8];
    UnkHashNode* next;
};

struct UnkHashMap;

struct UnkHashIter {
    UnkHashNode* node;
    UnkHashNode* prev;
    UnkHashMap* map;
    s32 bucket;
    u32 stamp;
};

struct UnkHashMap {
    u8 unk_0x0[0x8];
    u32 bucket_count;
    u32 modulus;
    u8 unk_0x10[0x38];
    u32 stamp;  // +0x48
};

static inline UnkHashIter MakeIter(UnkHashNode* node, UnkHashNode* prev, UnkHashMap* map, s32 bucket) {
    UnkHashIter it;
    it.node = node;
    it.prev = prev;
    it.map = map;
    it.bucket = bucket;
    it.stamp = map ? map->stamp : 0;
    return it;
}

template <UnkHashNode** (*Bucket)(UnkHashMap*, u32)>
__attribute__((always_inline)) static inline UnkHashIter Find(UnkHashMap* map, UnkHashKeyObj* const* key) {
    u32 b;
    if (map->bucket_count == 0 ||
        (b = ((*key)->field_0x30 ^ (*key)->field_0x34) % map->modulus) >= map->bucket_count)
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

ERASE_KEY(71002f7e04, 71002f7a2c, 71002f7efc)
ERASE_KEY(7100020eb0, 7100020cd0, 7100020fa8)
ERASE_KEY(7100021c04, 7100021a28, 7100021cfc)
ERASE_KEY(7100022958, 710002277c, 7100022a50)
ERASE_KEY(71000eca70, 71000ec350, 71000ecb68)
ERASE_KEY(710102c298, 710102bf0c, 710102c390)
ERASE_KEY(7101195894, 71011952f0, 710119598c)
ERASE_KEY(7101195b0c, 71011955d4, 7101195c04)
ERASE_KEY(7101195d84, 7101194aa8, 7101195e7c)
