// Unit 347: move-assignment of a named map of bound objects: takes the other's name and settings,
// rebinds each object to this map (re-inserting it by its key), then empties the other map.
// Names and layouts are placeholders.
#include "base/global/CRntDictionary.h"
#include "base/global/CStrId.h"
#include "game/UnkBoundObject_b260.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkBoundObject_b260;

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


UnkBoundMap_b260* fn_71002632a8(UnkBoundMap_b260* self, UnkBoundMap_b260* other) {
    self->name.ReleaseAndTake(other->name);
    self->map.field_0x48 = other->map.field_0x48;
    for (UnkIdNode_b260* node = other->map.first; node; node = node->next) {
        UnkBoundObject_b260* object = node->value;
        if (object->owner == other)
            object->owner = nullptr;
        object->owner = self;
        self->map.Insert(object->key, object);
    }
    other->map.Clear();
    return self;
}
