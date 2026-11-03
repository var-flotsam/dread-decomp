// From unit 2744 of tools/tu_map.py (the GUI scripts unit, gui/scripts/guimanager.lua): removes a
// named entry from a map of owned objects keyed by string id, destroying and freeing the object.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkHashedStr_71011b9bc4 {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

struct UnkObject_71011b9bc4;

struct UnkNode_71011b9bc4 {
    UnkStringData* key;
    UnkObject_71011b9bc4* value;
    u8 pad_0x10[0x10];
    UnkNode_71011b9bc4* next;  // +0x20
};

struct UnkMap_71011b9bc4 {
    u8 pad_0x0[0x10];
    UnkNode_71011b9bc4** buckets;  // +0x10
    u32 count;                     // +0x18
    u32 bucket_count;              // +0x1c

    u32 Bucket(UnkStringData* key) const {
        const UnkHashedStr_71011b9bc4* h = reinterpret_cast<const UnkHashedStr_71011b9bc4*>(key);
        return (h->hash_lo ^ h->hash_hi) % bucket_count;
    }
    bool Contains(UnkStringData* key) const {
        if (!count)
            return false;
        u32 b = Bucket(key);
        UnkNode_71011b9bc4** slot = buckets + b;
        if (!slot)
            return false;
        for (UnkNode_71011b9bc4* n = *slot; n; n = n->next)
            if (n->key == key)
                return true;
        return false;
    }
    UnkObject_71011b9bc4* At(UnkStringData* key) const {
        u32 b = Bucket(key);
        UnkNode_71011b9bc4* n = buckets[b];
        while (n->key != key)
            n = n->next;
        return n->value;
    }
};

struct UnkOwner_71011b9bc4 {
    u8 pad_0x0[0x8];
    UnkMap_71011b9bc4* map;  // +0x8
};

void fn_7100b7e8ac(UnkObject_71011b9bc4* obj);
void fn_7100b810d8(UnkMap_71011b9bc4* map, const CStrId* key);

void fn_71011b9bc4(UnkOwner_71011b9bc4* self, const char* const* name) {
    CStrId id(*name ? *name : "", true);
    if (self->map->Contains(id.data)) {
        if (UnkObject_71011b9bc4* obj = self->map->At(id.data)) {
            fn_7100b7e8ac(obj);
            fn_7100083c00(obj);
        }
        fn_7100b810d8(self->map, &id);
    }
}
