// base::reflection's unit (tools/tu_map.py unit 346): the copy constructor of the 0xc8-byte value of the
// reflected fixed arrays: a default value (as fn_710025b068 builds it), then every record of the other's id
// map and every node of the other's node map moved into it (as fn_710025b7f4). Names and layouts are
// placeholders.
#include <new>

#include "../../game/CMinimapData/UnkStrIdIterBeginEnd_A6_b75.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkCollectionIter.h"
#include "base/global/CRntDictionary.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::UnkAllocator;
using base::global::CRntDictionary;
using base::global::CRntSmallDictionary;
using base::global::CRntVector;
using base::global::CStrId;

void fn_7100262dd4(char*);

struct alignas(8) UnkReflRecord60_ovb0036 {
    u8 data[0x60];
    ~UnkReflRecord60_ovb0036() { fn_7100262dd4(reinterpret_cast<char*>(this)); }
};


template <u32 Flags>
struct UnkAllocatorHandle_ovb0036 {
    UnkAllocator* allocator = fn_7100083990();
    u32 flags = Flags;
    u32 field_0xc = 0;
};

template <class T>
struct UnkResizedVector_ovb0036 : CRntVector<T> {
    using CRntVector<T>::CRntVector;

    // Resizes to n entries, zeroing the new ones.
    __attribute__((always_inline)) void Resize(u32 n) {
        if (this->count < n) {
            T* p;
            if (this->capacity >= n) {
                p = this->data;
            } else {
                p = static_cast<T*>(this->allocator->vf_0x40((u64)n * sizeof(T), 8, this->flags));
                if (this->data) {
                    __builtin_memcpy(p, this->data, (u64)this->count * sizeof(T));
                    this->allocator->vf_0x48(this->data);
                }
                this->capacity = n;
                this->data = p;
            }
            __builtin_memset(p + this->count, 0, (u64)(n - this->count) * sizeof(T));
        } else if (this->count == n) {
            return;
        }
        this->count = n;
    }
};

// Eleven zeroed words with their allocator handle (flags `Flags`).
template <u32 Flags>
struct UnkWordSet_ovb0036 {
    UnkAllocatorHandle_ovb0036<Flags> handle;
    UnkResizedVector_ovb0036<u64> words;
    u64 field_0x38;
    u64 field_0x40;

    __attribute__((always_inline)) UnkWordSet_ovb0036() : words(Flags), field_0x38(0), field_0x40(0) {
        words.Resize(11);
    }
};

template <u32 Flags>
struct UnkWordList_ovb0036 {
    u64 field_0x0 = 0;
    u64 field_0x8 = 0;
    u64 field_0x10 = 0;
    u32 step = 10;
    u32 flags = Flags;
    UnkAllocator* allocator = fn_7100083990();
};

extern void* g_unk_7101a20c78[];

template <u32 Flags>
struct UnkReflValueC8Data_ovb0036 {
    void** vtable;
    UnkWordList_ovb0036<Flags> list;     // 0x8
    UnkWordSet_ovb0036<Flags> set_a;     // 0x30
    bool field_0x78;
    UnkWordSet_ovb0036<0xe> set_b;       // 0x80

    __attribute__((always_inline)) UnkReflValueC8Data_ovb0036() : vtable(g_unk_7101a20c78 + 2), field_0x78(true) {}
};
static_assert(sizeof(UnkReflValueC8Data_ovb0036<5>) == 0xc8, "layout");

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

// Moves every record and node of `other` into `self` (the assignment, fn_710025b7f4).
static inline void Append(char* self, char* other) {
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
}

void fn_710025b278(char* self, char* other) {
    new (self) UnkReflValueC8Data_ovb0036<5>;
    Append(self, other);
}
