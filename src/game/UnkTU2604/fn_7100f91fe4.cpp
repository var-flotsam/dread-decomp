// Unit 2604: inserts an entry into a chained, linked hash map keyed by string id: the key id is copied (its count
// raised), the record's id is moved in and its three fields are copied. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkTU2604InsRecord {
    UnkMovedStrId id;
    u64 field_0x8;
    u64 field_0x10;
    u32 field_0x18;

    UnkTU2604InsRecord(UnkTU2604InsRecord&& other) = default;
};

struct UnkTU2604InsHashed {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

struct UnkTU2604InsNode {
    CStrId key;
    UnkTU2604InsRecord value;  // 0x8
    UnkTU2604InsNode* next;    // 0x28
    UnkTU2604InsNode* prev;    // 0x30
    UnkTU2604InsNode* chain;   // 0x38

    UnkTU2604InsNode(const CStrId& k, UnkTU2604InsRecord& v)
        : key(k), value(static_cast<UnkTU2604InsRecord&&>(v)), next(nullptr), prev(nullptr), chain(nullptr) {}

    UnkTU2604InsNode*& Next() { return next; }
    UnkTU2604InsNode*& Prev() { return prev; }
};

struct UnkTU2604InsMap {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;  // 0xc
    UnkTU2604InsNode** buckets;
    u32 bucket_count;
    u32 divisor;
    u8 pad_0x20[0x18];
    UnkTU2604InsNode* tail;  // 0x38
    UnkTU2604InsNode* head;  // 0x40
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_7100f91fe4(UnkTU2604InsMap* map, const CStrId* key, UnkTU2604InsRecord* record) {
    UnkTU2604InsNode** bucket = nullptr;
    if (map->bucket_count != 0) {
        const UnkTU2604InsHashed* h = reinterpret_cast<const UnkTU2604InsHashed*>(key->data);
        u32 index = (h->hash_lo ^ h->hash_hi) % map->divisor;
        bucket = &map->buckets[index];
    }
    UnkTU2604InsNode* old = *bucket;
    auto* node = static_cast<UnkTU2604InsNode*>(map->allocator->vf_0x40(sizeof(UnkTU2604InsNode), 8, map->flags));
    if (node)
        new (node) UnkTU2604InsNode(*key, *record);
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
