// Unit 2560 (the billboard manager): inserts a node into the chained, linked hash map of drop-request lists
// keyed by string id: the key id is copied (its count raised) and the list of up to 16 requests copied with
// it. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkDrop_le1d620 {
    CStrId id;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    bool field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    f32 field_0x20;

    UnkDrop_le1d620(const UnkDrop_le1d620& other) = default;
};
static_assert(sizeof(UnkDrop_le1d620) == 0x28, "drop layout");

#pragma pack(push, 4)
// A fixed-capacity list: the requests are built in place as they are added.
struct UnkDropList_le1d620 {
    u32 count;
    u8 storage[16 * sizeof(UnkDrop_le1d620)];

    UnkDrop_le1d620* items() { return reinterpret_cast<UnkDrop_le1d620*>(storage); }
    const UnkDrop_le1d620* items() const { return reinterpret_cast<const UnkDrop_le1d620*>(storage); }

    UnkDropList_le1d620(const UnkDropList_le1d620& other) {
        for (u64 i = 0; i < other.count; ++i)
            new (&items()[i]) UnkDrop_le1d620(other.items()[i]);
        count = other.count;
    }
};
#pragma pack(pop)

struct UnkDropNode_le1d620 {
    CStrId key;
    UnkDropList_le1d620 list;
    UnkDropNode_le1d620* next;   // 0x290
    UnkDropNode_le1d620* prev;   // 0x298
    UnkDropNode_le1d620* chain;  // 0x2a0

    UnkDropNode_le1d620(const CStrId& k, const UnkDropList_le1d620& l)
        : key(k), list(l), next(nullptr), prev(nullptr), chain(nullptr) {}

    UnkDropNode_le1d620*& Next() { return next; }
    UnkDropNode_le1d620*& Prev() { return prev; }
};
static_assert(sizeof(UnkDropNode_le1d620) == 0x2a8, "node layout");

struct UnkHashed_le1d620 {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

struct UnkDropMap_le1d620 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;  // 0xc
    UnkDropNode_le1d620** buckets;
    u32 bucket_count;
    u32 divisor;
    u8 pad_0x20[0x18];
    UnkDropNode_le1d620* tail;  // 0x38
    UnkDropNode_le1d620* head;  // 0x40
};

UnkDropNode_le1d620* fn_7100e1d620(UnkDropMap_le1d620* map, const CStrId* key, const UnkDropList_le1d620* list) {
    UnkDropNode_le1d620** bucket = nullptr;
    if (map->bucket_count != 0) {
        const UnkHashed_le1d620* h = reinterpret_cast<const UnkHashed_le1d620*>(key->data);
        u32 index = (h->hash_lo ^ h->hash_hi) % map->divisor;
        bucket = &map->buckets[index];
    }
    UnkDropNode_le1d620* old = *bucket;
    auto* node =
        static_cast<UnkDropNode_le1d620*>(map->allocator->vf_0x40(sizeof(UnkDropNode_le1d620), 8, map->flags));
    if (node)
        new (node) UnkDropNode_le1d620(*key, *list);
    node->chain = old;
    *bucket = node;
    if (!map->head)
        map->head = node;
    else if (map->tail) {
        node->Prev() = map->tail;
        map->tail->Next() = node;
    }
    map->tail = node;
    ++map->count;
    return node;
}
