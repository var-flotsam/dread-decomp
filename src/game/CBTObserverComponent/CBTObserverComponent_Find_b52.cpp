// CBTObserverComponent unit (tools/tu_map.py unit 2216): a copy of fn_71011b8724
// (UnkUnit2744_Find_b51.cpp). Looks an entry up by string id in the chained hash map at +0x8 and
// returns its value (inline in the node, after the key). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkHashedStr_7100ca7634 {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

struct UnkNode_7100ca7634 {
    UnkStringData* key;
    u8 value[0x38];            // +0x8
    UnkNode_7100ca7634* next;  // +0x40
};

struct UnkMap_7100ca7634 {
    u8 pad_0x0[0x10];
    UnkNode_7100ca7634** buckets;  // +0x10
    u32 count;                     // +0x18
    u32 bucket_count;              // +0x1c

    UnkNode_7100ca7634* Find(const CStrId& key) const {
        if (!count)
            return nullptr;
        const UnkHashedStr_7100ca7634* h = reinterpret_cast<const UnkHashedStr_7100ca7634*>(key.data);
        u32 b = (h->hash_lo ^ h->hash_hi) % bucket_count;
        UnkNode_7100ca7634** slot = buckets + b;
        if (!slot)
            return nullptr;
        for (UnkNode_7100ca7634* n = *slot; n; n = n->next)
            if (n->key == key.data)
                return n;
        return nullptr;
    }
};

struct UnkOwner_7100ca7634 {
    u8 pad_0x0[0x8];
    UnkMap_7100ca7634 map;  // +0x8
};

void* fn_7100ca7634(UnkOwner_7100ca7634* self, const CStrId* key) {
    UnkNode_7100ca7634* n = self->map.Find(*key);
    return n ? n->value : nullptr;
}
