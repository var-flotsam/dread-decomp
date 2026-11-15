// Unit 2604: assigns a record (string id and three fields) to the entry of a hashed map that a string id names,
// when there is one: the entry's id is released and takes the record's, which keeps the empty id.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkTU2604HashedStr {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

struct UnkTU2604AssignData {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;
};

struct UnkTU2604AssignRecord {
    CStrId id;
    UnkTU2604AssignData data;
};

struct UnkTU2604AssignNode {
    UnkStringData* key;
    UnkTU2604AssignRecord value;  // 0x8
    u8 pad_0x28[0x10];
    UnkTU2604AssignNode* next;  // 0x38
};

struct UnkTU2604AssignMap {
    u8 pad_0x0[0x10];
    UnkTU2604AssignNode** buckets;
    u32 count;
    u32 bucket_count;

    UnkTU2604AssignNode** BucketFor(UnkStringData* key) {
        if (count == 0)
            return nullptr;
        const UnkTU2604HashedStr* h = reinterpret_cast<const UnkTU2604HashedStr*>(key);
        u32 index = (h->hash_lo ^ h->hash_hi) % bucket_count;
        return buckets + index;
    }
    UnkTU2604AssignNode* Find(UnkStringData* key) {
        UnkTU2604AssignNode** slot = BucketFor(key);
        if (!slot)
            return nullptr;
        for (UnkTU2604AssignNode* n = *slot; n; n = n->next)
            if (n->key == key)
                return n;
        return nullptr;
    }
};

void fn_7100f92270(UnkTU2604AssignMap* map, const CStrId* key, UnkTU2604AssignRecord* record) {
    UnkTU2604AssignNode* node = map->Find(key->data);
    if (!node)
        return;
    UnkTU2604AssignData data;
    node->value.id.ReleaseAndTake(record->id);
    data.field_0x8 = record->data.field_0x8;
    data.field_0x0 = record->data.field_0x0;
    data.field_0x10 = record->data.field_0x10;
    node->value.data.field_0x10 = data.field_0x10;
    node->value.data.field_0x0 = data.field_0x0;
    node->value.data.field_0x8 = data.field_0x8;
}
