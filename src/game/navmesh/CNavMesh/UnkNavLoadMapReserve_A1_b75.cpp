// Unit 355 (navmesh::CNavMesh's unit): the reserves of two maps of maps of
// src/game/navmesh/CNavMesh.cpp (UnkNavLoadMap: values and string-id keys in two arrays), in the
// shape of src/game/CMinimapDef_ShrinkToFit_b49.cpp's fn_7100e71984: the inner maps are moved over
// (the moved-from ones keep their capacity, growth and allocator) and destroyed inline (emptied
// out of line, then both arrays freed), the keys taken over and released in two steps, and the
// old arrays freed. Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneRecords.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkObject_0x100.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// The types of CNavMesh.cpp's declarations.
template <class V>
struct UnkNavLoadMap;
struct UnkNavLoadSlot360;
struct UnkNavLoadPathRecord;
struct UnkClone_fn_710027ead8;
extern template void base::UnkKeyedVecArray<UnkClone_fn_710027ead8>::Clear();

namespace navmesh_load_a1_b75 {

// A map's layout, as an element of the outer map.
struct UnkInnerMap {
    void* values;
    void* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkInnerMap(UnkInnerMap&& other)
        : values(other.values), keys(other.keys), count(other.count), capacity(other.capacity), step(other.step),
          flags(other.flags), allocator(other.allocator) {
        other.count = 0;
        other.keys = nullptr;
        other.values = nullptr;
    }
};
static_assert(sizeof(UnkInnerMap) == 0x28, "UnkInnerMap layout");

// A key as the reserve moves it: the moved-from id is cleared through a union, then released in
// two steps.
union UnkKeyIdSlot {
    UnkStringData* p;
};

struct UnkKeyId {
    UnkStringData* data;

    UnkKeyId(UnkKeyId&& other) : data(other.data) {
        reinterpret_cast<UnkKeyIdSlot*>(&other.data)->p = &g_unk_7101d47108->empty;
    }
    ~UnkKeyId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkOuterMap {
    UnkInnerMap* values;
    UnkKeyId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

template <void (*Clear)(UnkInnerMap*)>
__attribute__((always_inline)) static inline void Reserve(UnkOuterMap* self, u32 capacity) {
    if (self->capacity >= capacity)
        return;
    auto* values =
        static_cast<UnkInnerMap*>(self->allocator->vf_0x40((u64)capacity * sizeof(UnkInnerMap), 8, self->flags));
    auto* keys = static_cast<UnkKeyId*>(self->allocator->vf_0x40((u64)capacity * sizeof(UnkKeyId), 8, self->flags));
    if (UnkInnerMap* src = self->values) {
        UnkInnerMap* dst = values;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) UnkInnerMap(static_cast<UnkInnerMap&&>(*src));
            Clear(src);
            if (src->keys)
                src->allocator->vf_0x48(src->keys);
            if (src->values)
                src->allocator->vf_0x48(src->values);
        }
        self->allocator->vf_0x48(self->values);
    }
    if (UnkKeyId* old = self->keys) {
        u64 j = 0;
        for (u32 i = 0; i < self->count; ++i, ++j, ++old) {
            new (&keys[j]) UnkKeyId(static_cast<UnkKeyId&&>(*old));
            old->~UnkKeyId();
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->values = values;
    self->keys = keys;
    self->capacity = capacity;
}

}  // namespace navmesh_load_a1_b75

using navmesh_load_a1_b75::UnkInnerMap;
using navmesh_load_a1_b75::UnkOuterMap;

using UnkNavLoadRecordMap = UnkNavLoadMap<base::UnkVector<UnkNavLoadPathRecord>>;
void fn_710027fb10(UnkNavLoadRecordMap* self);  // as CNavMesh.cpp declares it

static inline void ClearCellRecordMap(UnkInnerMap* map) {
    reinterpret_cast<base::UnkKeyedVecArray<UnkClone_fn_710027ead8>*>(map)->Clear();
}
static inline void ClearRecordMap(UnkInnerMap* map) {
    fn_710027fb10(reinterpret_cast<UnkNavLoadMap<base::UnkVector<UnkNavLoadPathRecord>>*>(map));
}

void fn_710027f348(UnkNavLoadMap<UnkNavLoadMap<base::UnkVector<UnkNavLoadSlot360>>>* self, u32 capacity) {
    navmesh_load_a1_b75::Reserve<ClearCellRecordMap>(reinterpret_cast<UnkOuterMap*>(self), capacity);
}

void fn_710027fc88(UnkNavLoadMap<UnkNavLoadMap<base::UnkVector<UnkNavLoadPathRecord>>>* self, u32 capacity) {
    navmesh_load_a1_b75::Reserve<ClearRecordMap>(reinterpret_cast<UnkOuterMap*>(self), capacity);
}

// The outer map's clear (fn_710027f8ac): the keys released, each inner map destroyed as above,
// the count zeroed; the arrays are kept.
namespace navmesh_load_a1_b75 {
struct UnkRecordMap : UnkInnerMap {
    __attribute__((always_inline)) ~UnkRecordMap() {
        ClearRecordMap(this);
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};
struct UnkRecordMaps {
    UnkRecordMap* values;
    base::UnkStrIdMember* keys;
    u32 count;
};
}  // namespace navmesh_load_a1_b75

void fn_710027f8ac(UnkNavLoadMap<UnkNavLoadMap<base::UnkVector<UnkNavLoadPathRecord>>>* self) {
    using base::UnkStrIdMember;
    auto* map = reinterpret_cast<navmesh_load_a1_b75::UnkRecordMaps*>(self);
    if (map->keys && map->count)
        for (u64 i = 0; i < map->count; ++i)
            map->keys[i].~UnkStrIdMember();
    if (map->values && map->count)
        for (u32 i = 0; i < map->count; ++i)
            map->values[i].~UnkRecordMap();
    map->count = 0;
}

// The inner map's clear (fn_710027fb10): the keys released, each record array freed (its records,
// a word and a 0x100-byte object, destroyed), the count zeroed; the arrays are kept.
namespace navmesh_load_a1_b75 {
struct UnkPathRecord {
    u32 field_0x0;
    UnkObject_0x100 data;
};
struct UnkPathRecordMap {
    base::UnkVector<UnkPathRecord>* values;
    base::UnkStrIdMember* keys;
    u32 count;
};
}  // namespace navmesh_load_a1_b75

void fn_710027fb10(UnkNavLoadMap<base::UnkVector<UnkNavLoadPathRecord>>* self) {
    using base::UnkStrIdMember;
    auto* map = reinterpret_cast<navmesh_load_a1_b75::UnkPathRecordMap*>(self);
    if (map->keys && map->count)
        for (u64 i = 0; i < map->count; ++i)
            map->keys[i].~UnkStrIdMember();
    if (map->values && map->count)
        for (u32 i = 0; i < map->count; ++i)
            map->values[i].Free();
    map->count = 0;
}

// The reserve of an array (base::UnkVector's layout) of the outer maps (fn_710027f9d0): moved over
// as in the reserves above, each destroyed by fn_710027f8ac and its two arrays freed.
namespace navmesh_load_a1_b75 {
struct UnkMapArray {
    UnkInnerMap* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};
}  // namespace navmesh_load_a1_b75

void fn_710027f9d0(base::UnkVector<UnkNavLoadMap<UnkNavLoadMap<base::UnkVector<UnkNavLoadPathRecord>>>>* self,
                   u32 capacity) {
    auto* v = reinterpret_cast<navmesh_load_a1_b75::UnkMapArray*>(self);
    if (v->capacity >= capacity)
        return;
    auto* data =
        static_cast<UnkInnerMap*>(v->allocator->vf_0x40((u64)capacity * sizeof(UnkInnerMap), 8, v->flags));
    if (UnkInnerMap* src = v->data) {
        UnkInnerMap* dst = data;
        for (u32 i = 0; i < v->count; ++i, ++dst, ++src) {
            new (dst) UnkInnerMap(static_cast<UnkInnerMap&&>(*src));
            fn_710027f8ac(reinterpret_cast<UnkNavLoadMap<UnkNavLoadMap<base::UnkVector<UnkNavLoadPathRecord>>>*>(src));
            if (src->keys)
                src->allocator->vf_0x48(src->keys);
            if (src->values)
                src->allocator->vf_0x48(src->values);
        }
        v->allocator->vf_0x48(v->data);
    }
    v->data = data;
    v->capacity = capacity;
}
