// The records of base::core::CAsset's unit (tools/tu_map.py unit 24): a map of string ids to
// 0x80-byte records, each holding two maps of ids to id pairs, an array of file-path ids and a
// file-path id. src/base/core/CAsset/fn_7100018558.cpp assigns such maps and names the types; this file
// holds the map's reserve (fn_7100017948) and the record's destructor (fn_7100017ce8).

#include <new>

#include "base/UnkDestructors.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

// String ids released in two steps: the record map's keys (default pool), and the keys of the
// record's two maps and its paths (file-path pool). The two maps hold base/UnkStrIdPathPair.h's
// pairs.
using UnkAssetStrId = base::UnkStrIdMember;
using UnkAssetPathId = base::UnkStrIdMemberOf<g_unk_7101d47200>;

// Values and keys in two arrays sharing one count (the layout of fn_7100018558.cpp's UnkIdMap).
template <class V, class K>
struct UnkAssetKeyedMap {
    V* values;
    K* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkAssetKeyedMap(UnkAssetKeyedMap&& other)
        : values(other.values), keys(other.keys), count(other.count), capacity(other.capacity),
          step(other.step), flags(other.flags), allocator(other.allocator) {
        other.count = 0;
        other.values = nullptr;
        other.keys = nullptr;
    }
    __attribute__((always_inline)) ~UnkAssetKeyedMap() {
        if (keys)
            for (u64 i = 0; i < count; ++i)
                keys[i].~K();
        if (values)
            for (u32 i = 0; i < count; ++i)
                values[i].~V();
        count = 0;
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};

// An array of file-path ids (base::UnkVector's layout), freed with its elements.
struct UnkAssetPathIdVector {
    UnkAssetPathId* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkAssetPathIdVector(UnkAssetPathIdVector&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    ~UnkAssetPathIdVector() {
        if (data) {
            for (u32 i = 0; i < count; ++i)
                data[i].~UnkAssetPathId();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkRecord80 {
    UnkAssetKeyedMap<UnkStrIdPathPair, UnkAssetPathId> field_0x0;
    UnkAssetKeyedMap<UnkStrIdPathPair, UnkAssetPathId> field_0x28;
    UnkAssetPathIdVector field_0x50;
    UnkAssetPathId field_0x78;

    UnkRecord80(UnkRecord80&& other) = default;
    // Inlined into fn_7100017ce8 only.
    __attribute__((always_inline)) ~UnkRecord80() {}
};
static_assert(sizeof(UnkRecord80) == 0x80, "UnkRecord80 layout");

// The map of string ids to records (fn_7100018558.cpp's UnkIdMap<UnkRecord80>).
template <class V>
struct UnkIdMap {
    V* values;
    UnkAssetStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

// The record's destructor.
// NON_MATCHING(minor): in the loops over the two maps' id pairs the counter and the byte offset
// are incremented in the other order (two independent adds swapped).
void fn_7100017ce8(UnkRecord80* record) {
    record->~UnkRecord80();
}

// Grows both arrays to `capacity` entries, moving the records and keys over.
// NON_MATCHING(minor): in the key loop the original compares the low 32 bits of the counter
// (cmp w, w instead of cmp x, w uxtw) and keeps the moved-from pool in x8 (ours x9, with the
// other temporaries one register up); the same loads, stores and calls.
void fn_7100017948(UnkIdMap<UnkRecord80>* self, u32 capacity) {
    if (self->capacity < capacity) {
        UnkRecord80* values = static_cast<UnkRecord80*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkRecord80), 8, self->flags));
        UnkAssetStrId* keys = static_cast<UnkAssetStrId*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkAssetStrId), 8, self->flags));
        if (self->values) {
            UnkRecord80* src = self->values;
            UnkRecord80* dst = values;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                new (dst) UnkRecord80(static_cast<UnkRecord80&&>(*src));
                fn_7100017ce8(src);
            }
            self->allocator->vf_0x48(self->values);
        }
        if (self->keys) {
            UnkAssetStrId* src = self->keys;
            for (u32 i = 0; i < self->count; ++i, ++src) {
                new (&keys[i]) UnkAssetStrId(static_cast<UnkAssetStrId&&>(*src));
                src->~UnkAssetStrId();
            }
            self->allocator->vf_0x48(self->keys);
        }
        self->values = values;
        self->keys = keys;
        self->capacity = capacity;
    }
}
