// From unit 2572 of tools/tu_map.py (no reflected class): whether a hashed set (at +0x48) holds the
// object a handle points to (at +0x10). Nodes chain at +0x10 with the object first; an object
// hashes by its 64-bit id at +0x30 (the two halves xored), modulo the divisor at +0x64, and a
// bucket past the count at +0x60 is empty. The shape of fn_710119daa0
// (src/game_hi/fn_710119daa0.cpp). Names and layouts are placeholders.
#include "types.h"

struct UnkHashedObject {
    u8 pad_0x0[0x30];
    u32 id_lo;
    u32 id_hi;
};

struct UnkHashedHandle {
    u8 pad_0x0[0x10];
    UnkHashedObject* object;
};

struct UnkHashedNode {
    UnkHashedObject* object;
    u8 pad_0x8[0x8];
    UnkHashedNode* next;
};

struct UnkHashedSet {
    u64 field_0x0;
    u64 field_0x8;
    UnkHashedNode** buckets;
    u32 count;
    u32 divisor;

    UnkHashedNode* Find(UnkHashedObject* object) {
        if (!count)
            return nullptr;
        u32 index = (object->id_lo ^ object->id_hi) % divisor;
        if (index >= count)
            return nullptr;
        UnkHashedNode* node = buckets[index];
        while (node && node->object != object)
            node = node->next;
        return node;
    }
};

struct UnkHashedSetOwner {
    u8 pad_0x0[0x48];
    UnkHashedSet set;
};

bool fn_7100ecafa8(UnkHashedSetOwner* self, UnkHashedHandle* handle) { return self->set.Find(handle->object) != nullptr; }
