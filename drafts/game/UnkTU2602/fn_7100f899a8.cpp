// Unit 2602 of tools/tu_map.py: the entry kept under a string id in a dictionary, added when there is none:
// a new entry is built, moved into the dictionary, named after the id, and the id is added to its list.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkPooledStrId.h"
#include "base/global/CRntDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

using UnkEntryId_lf899a8 = base::UnkPooledStrId<g_unk_7101d47108>;

struct UnkEntryElem_lf899a8;

// The dictionary's value (0x68 bytes): built and destroyed out of line.
struct UnkEntry_lf899a8 {
    UnkEntryId_lf899a8 name;                     // 0x0
    CRntVector<CStrId> ids;                      // 0x8
    UnkEntryId_lf899a8 field_0x30;               // 0x30
    u32 field_0x38;                              // 0x38
    CRntVector<UnkEntryElem_lf899a8*> elems;     // 0x40

    UnkEntry_lf899a8() asm("fn_7100f85a78");
    UnkEntry_lf899a8(UnkEntry_lf899a8&& other) = default;
    ~UnkEntry_lf899a8() asm("fn_7100d90304");

    void SetName(UnkEntryId_lf899a8 id) { name = id; }
};
static_assert(sizeof(UnkEntry_lf899a8) == 0x68, "layout");

using UnkEntryMap_lf899a8 = base::global::CRntDictionary<CStrId, UnkEntry_lf899a8>;

// Adds an id to the entry's list (called as (list, id)).
void fn_7100fed650(char*, char*);

// Moves a new entry into the dictionary under key: a node at the front of the key's bucket and at the end
// of the list. Unlike the dictionary's own Insert, the new node is not checked for null.
static inline UnkEntryMap_lf899a8::TNode* Add(UnkEntryMap_lf899a8* map, const CStrId& key, UnkEntry_lf899a8&& entry) {
    using TNode = UnkEntryMap_lf899a8::TNode;
    TNode** bucket = nullptr;
    if (map->buckets.count != 0) {
        u32 index = base::global::CRntDictionaryHash<CStrId>::Of(key) % map->buckets.capacity;
        bucket = map->buckets.data + index;
    }
    TNode* old = *bucket;
    TNode* node = new (map->allocator->vf_0x40(sizeof(TNode), 8, map->flags))
        TNode{key, static_cast<UnkEntry_lf899a8&&>(entry), nullptr, nullptr, old};
    *bucket = node;
    if (!map->first)
        map->first = node;
    else if (map->last) {
        node->Prev() = map->last;
        map->last->Next() = node;
    }
    map->last = node;
    ++map->count;
    return node;
}

UnkEntry_lf899a8& fn_7100f899a8(UnkEntryMap_lf899a8* map, const CStrId& key) {
    if (map->Find(key))
        return map->FindNode(key)->value;
    UnkEntry_lf899a8& entry = Add(map, key, UnkEntry_lf899a8())->value;
    entry.SetName(UnkEntryId_lf899a8(key.data));
    fn_7100fed650(reinterpret_cast<char*>(&entry.ids), reinterpret_cast<char*>(const_cast<CStrId*>(&key)));
    return entry;
}
