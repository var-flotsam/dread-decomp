// From unit 2744 of tools/tu_map.py (the GUI scripts unit): looks an entry up by string id in the
// chained hash map at +0x140 and returns its value (inline in the node, after the key). Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkHashedStr_71011b8724 {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

struct UnkNode_71011b8724 {
    UnkStringData* key;
    u8 value[0x50];            // +0x8
    UnkNode_71011b8724* next;  // +0x58
};

struct UnkMap_71011b8724 {
    u8 pad_0x0[0x10];
    UnkNode_71011b8724** buckets;  // +0x10
    u32 count;                     // +0x18
    u32 bucket_count;              // +0x1c

    UnkNode_71011b8724* Find(const CStrId& key) const {
        if (!count)
            return nullptr;
        const UnkHashedStr_71011b8724* h = reinterpret_cast<const UnkHashedStr_71011b8724*>(key.data);
        u32 b = (h->hash_lo ^ h->hash_hi) % bucket_count;
        UnkNode_71011b8724** slot = buckets + b;
        if (!slot)
            return nullptr;
        for (UnkNode_71011b8724* n = *slot; n; n = n->next)
            if (n->key == key.data)
                return n;
        return nullptr;
    }
};

struct UnkOwner_71011b8724 {
    u8 pad_0x0[0x140];
    UnkMap_71011b8724 map;  // +0x140
};

void* fn_71011b8724(UnkOwner_71011b8724* self, const CStrId* key) {
    UnkNode_71011b8724* n = self->map.Find(*key);
    return n ? n->value : nullptr;
}
