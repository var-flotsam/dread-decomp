// Unit 2604: inserts an entry into a chained, linked hash map keyed by string id: the key id is copied (its count
// raised), the record's id is copied too and its three fields with it. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkTU2604InsBRecord {
    CStrId id;
    u64 field_0x8;
    u64 field_0x10;
    u32 field_0x18;

    UnkTU2604InsBRecord(const UnkTU2604InsBRecord& other) = default;
};

struct UnkTU2604InsBHashed {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

struct UnkTU2604InsBNode {
    CStrId key;
    UnkTU2604InsBRecord value;  // 0x8
    UnkTU2604InsBNode* next;    // 0x28
    UnkTU2604InsBNode* prev;    // 0x30
    UnkTU2604InsBNode* chain;   // 0x38

    UnkTU2604InsBNode(const CStrId& k, const UnkTU2604InsBRecord& v)
        : key(k), value(v), next(nullptr), prev(nullptr), chain(nullptr) {}

    UnkTU2604InsBNode*& Next() { return next; }
    UnkTU2604InsBNode*& Prev() { return prev; }
};

struct UnkTU2604InsBMap {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;  // 0xc
    UnkTU2604InsBNode** buckets;
    u32 bucket_count;
    u32 divisor;
    u8 pad_0x20[0x18];
    UnkTU2604InsBNode* tail;  // 0x38
    UnkTU2604InsBNode* head;  // 0x40
};

void fn_7100f91ec0(UnkTU2604InsBMap* map, const CStrId* key, const UnkTU2604InsBRecord* record) {
    UnkTU2604InsBNode** bucket = nullptr;
    if (map->bucket_count != 0) {
        const UnkTU2604InsBHashed* h = reinterpret_cast<const UnkTU2604InsBHashed*>(key->data);
        u32 index = (h->hash_lo ^ h->hash_hi) % map->divisor;
        bucket = &map->buckets[index];
    }
    UnkTU2604InsBNode* old = *bucket;
    auto* node = static_cast<UnkTU2604InsBNode*>(map->allocator->vf_0x40(sizeof(UnkTU2604InsBNode), 8, map->flags));
    if (node)
        new (node) UnkTU2604InsBNode(*key, *record);
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
}
