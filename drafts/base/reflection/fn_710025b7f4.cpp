// base::reflection's unit (tools/tu_map.py unit 346): the assignment of the 0xc8-byte value of the reflected
// fixed arrays: every record of the other's id map is moved into this one's, then every node of the other's
// node map is moved into this one's. Names and layouts are placeholders.
#include <new>

#include "../../game/CMinimapData/UnkStrIdIterBeginEnd_A6_b75.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkCollectionIter.h"
#include "base/global/CRntDictionary.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntDictionary;
using base::global::CRntSmallDictionary;
using base::global::CRntVector;
using base::global::CStrId;

void fn_7100262dd4(char*);

struct alignas(8) UnkReflRecord60_ovb0036 {
    u8 data[0x60];
    ~UnkReflRecord60_ovb0036() { fn_7100262dd4(reinterpret_cast<char*>(this)); }
};

struct UnkMapRecord_ovb0036;
void fn_7100262f74(UnkMapRecord_ovb0036* self, UnkMapRecord_ovb0036* other);  // move constructor

struct UnkStruct_710025fdf4;
void fn_710025fdf4(UnkStruct_710025fdf4* self, u32 capacity);  // Reserve

// A dictionary of vectors by string id; the inserts move the vector into the node.
using UnkVectorMap = CRntDictionary<CStrId, CRntVector<void*>>;
UnkVectorMap::TNode* fn_71002487ac(UnkVectorMap* self, const CStrId& key, CRntVector<void*>& value);

struct UnkNodeValueTag_l25b7f4;
// The node map's values as the iterator holds them: arrays of 0x40-byte polymorphic elements.
using UnkNodeValue_l25b7f4 = UnkClonePolyVec<UnkNodeValueTag_l25b7f4>;

struct UnkNodeIter_l25b7f4;
void fn_710023c550(UnkNodeIter_l25b7f4* it);  // re-points `current` at the node

// An iterator over the node map's list, caching the node's (key, value).
struct UnkNodeIter_l25b7f4 {
    UnkVectorMap::TNode* node;
    base::UnkIterValue<UnkNodeValue_l25b7f4, CStrId> current;

    explicit UnkNodeIter_l25b7f4(UnkVectorMap::TNode* node) : node(node) { fn_710023c550(this); }
    bool operator!=(const UnkNodeIter_l25b7f4& other) const { return node != other.node; }
    void Next() {
        node = node->next;
        fn_710023c550(this);
    }
};

using UnkRecordMap_l25b7f4 = CRntSmallDictionary<CStrId, UnkReflRecord60_ovb0036>;
using UnkRecordIter_l25b7f4 = UnkStrIdIterValue_A6_b75<UnkReflRecord60_ovb0036>;

struct UnkReflValueC8_l25b7f4 {
    void* field_0x0;
    UnkRecordMap_l25b7f4 records;  // 0x8
    UnkVectorMap nodes;            // 0x30
};
static_assert(__builtin_offsetof(UnkReflValueC8_l25b7f4, nodes) == 0x30, "layout");

// Appends a key and the record moved out of `record`.
static inline void Add(UnkRecordMap_l25b7f4* map, const CStrId& key, UnkReflRecord60_ovb0036* record) {
    if (map->count == map->capacity)
        fn_710025fdf4(reinterpret_cast<UnkStruct_710025fdf4*>(map), map->step + map->count);
    new (&map->keys[map->count]) CStrId(key);
    fn_7100262f74(reinterpret_cast<UnkMapRecord_ovb0036*>(&map->values[map->count]),
                  reinterpret_cast<UnkMapRecord_ovb0036*>(record));
    ++map->count;
}

char* fn_710025b7f4(char* self, char* other) {
    UnkReflValueC8_l25b7f4* to = reinterpret_cast<UnkReflValueC8_l25b7f4*>(self);
    UnkReflValueC8_l25b7f4* from = reinterpret_cast<UnkReflValueC8_l25b7f4*>(other);

    {
        UnkRecordIter_l25b7f4 it(from->records.keys, from->records.values);
        while (it.ref.key != &from->records.keys[from->records.count]) {
            CStrId* key = it.state == 2 ? it.ref.key : &it.value.key;
            UnkReflRecord60_ovb0036* record = it.state == 2 ? it.ref.element : &it.value.element;
            Add(&to->records, *key, record);
            ++it.ref.key;
            ++it.ref.element;
        }
    }

    for (UnkNodeIter_l25b7f4 it(from->nodes.first); it != UnkNodeIter_l25b7f4(nullptr); it.Next())
        fn_71002487ac(&to->nodes, it.current.Index(), reinterpret_cast<CRntVector<void*>&>(it.current.Element()));
    return self;
}
