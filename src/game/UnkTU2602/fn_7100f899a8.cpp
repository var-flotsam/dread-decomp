// Unit 2602 of tools/tu_map.py: the entry kept under a string id in a dictionary, added when there is none:
// a new entry is built, moved into the dictionary, named after the id, and the id is added to its list.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/sound/UnkMusicMapValue.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

using UnkEntryMap_lf899a8 = base::global::CRntDictionary<CStrId, UnkMusicMapValue>;

// Names the value after a copy of the id.
static void SetName(UnkMusicMapValue& value, CStrId id) {
    value.name = reinterpret_cast<const base::UnkStrIdMember&>(id);
}

// Moves a new entry into the dictionary under key: a node at the front of the key's bucket and at the end
// of the list. Unlike the dictionary's own Insert, the new node is not checked for null.
static inline UnkEntryMap_lf899a8::TNode* Add(UnkEntryMap_lf899a8* map, const CStrId& key, UnkMusicMapValue&& entry) {
    using TNode = UnkEntryMap_lf899a8::TNode;
    TNode** bucket = nullptr;
    if (map->buckets.count != 0) {
        u32 index = base::global::CRntDictionaryHash<CStrId>::Of(key) % map->buckets.capacity;
        bucket = map->buckets.data + index;
    }
    TNode* old = *bucket;
    TNode* node = new (map->allocator->vf_0x40(sizeof(TNode), 8, map->flags))
        TNode{key, static_cast<UnkMusicMapValue&&>(entry), nullptr, nullptr, old};
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

UnkMusicMapValue& fn_7100f899a8(UnkEntryMap_lf899a8* map, const CStrId& key) {
    if (map->Find(key))
        return map->FindNode(key)->value;
    UnkMusicMapValue& entry = Add(map, key, UnkMusicMapValue())->value;
    SetName(entry, key);
    fn_7100fed650(reinterpret_cast<char*>(&entry.layer_set.layers), reinterpret_cast<char*>(const_cast<CStrId*>(&key)));
    return entry;
}
