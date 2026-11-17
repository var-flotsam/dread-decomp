// Unit 347: move constructor of a named map of bound objects: copies the other's name, builds an empty map
// with eleven buckets, rebinds each of the other's objects to this map, then empties the other map.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntDictionary.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "types.h"

using base::global::CStrId;

struct UnkBoundObject_b260;

struct UnkBoundObject_b260 {
    u8 unk_0x0[0x30];
    CStrId key;   // 0x30
    u8 unk_0x38[0x90];
    void* owner;  // 0xc8
    u8 unk_0xd0[0xe5];
    u8 detached;  // 0x1b5
};

// The bound objects by id, and a word after them.
struct UnkIdMap_b260 : base::global::CRntDictionary<CStrId, UnkBoundObject_b260*> {
    u64 field_0x48;
};
using UnkIdNode_b260 = UnkIdMap_b260::TNode;

struct UnkBoundMap_b260 {
    void* vtable;                   // 0x0
    CStrId name;                    // 0x8
    UnkIdMap_b260 map;              // 0x10
};
static_assert(__builtin_offsetof(UnkBoundMap_b260, map.first) == 0x50, "layout");

using UnkIdDictionary_b260 = base::global::CRntDictionary<CStrId, UnkBoundObject_b260*>;

// The callers name the map as an opaque record (reflection_recordmaps_ovb0036.cpp).
struct UnkMapRecord_ovb0036;

void fn_7100262f74(UnkMapRecord_ovb0036* raw_self, UnkMapRecord_ovb0036* raw_other) {
    UnkBoundMap_b260* self = reinterpret_cast<UnkBoundMap_b260*>(raw_self);
    UnkBoundMap_b260* other = reinterpret_cast<UnkBoundMap_b260*>(raw_other);
    self->vtable = reinterpret_cast<char*>(g_unk_7101a20c98) + 0x10;
    new (&self->name) CStrId(other->name);
    new (static_cast<UnkIdDictionary_b260*>(&self->map)) UnkIdDictionary_b260(0xe);
    self->map.ResizeBuckets(11);
    self->map.field_0x48 = other->map.field_0x48;
    for (UnkIdNode_b260* node = other->map.first; node; node = node->next) {
        UnkBoundObject_b260* object = node->value;
        if (object->owner == other)
            object->owner = nullptr;
        object->owner = self;
        self->map.Insert(object->key, object);
    }
    other->map.Clear();
}
