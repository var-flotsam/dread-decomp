// From unit 2734 of tools/tu_map.py (no reflected class): whether a hashed set of objects holds the
// given one. Nodes chain at +0x50 with the object first; an object hashes by its 64-bit id at +0x30
// (the two halves xored), modulo the bucket count. Names and layouts are placeholders.
#include "types.h"

struct UnkSetObject {
    u8 pad_0x0[0x30];
    u32 id_lo;
    u32 id_hi;
};

struct UnkSetNode {
    UnkSetObject* object;
    u8 pad_0x8[0x48];
    UnkSetNode* next;
};

struct UnkObjectSet {
    u64 field_0x0;
    u64 field_0x8;
    UnkSetNode** buckets;
    u32 count;
    u32 bucket_count;

    UnkSetNode* Find(UnkSetObject* object) {
        if (!count)
            return nullptr;
        u32 hash = object->id_lo ^ object->id_hi;
        u32 index = hash % bucket_count;
        UnkSetNode** bucket = &buckets[index];
        if (!bucket)
            return nullptr;
        UnkSetNode* node = *bucket;
        while (node && node->object != object)
            node = node->next;
        return node;
    }
};

bool fn_710119daa0(UnkObjectSet* self, UnkSetObject* const* object) { return self->Find(*object) != nullptr; }
