// Unit 355 (navmesh::CNavMesh's unit): the clear of the outer map of cell record maps
// (fn_710027e818), a copy of fn_710027f8ac (src/game/navmesh/CNavMesh/UnkNavLoadMapReserve_A1_b75.cpp)
// whose inner maps are emptied by the clone Clear fn_710027ead8: the keys released, each inner map
// emptied and its two arrays freed, the count zeroed; the arrays are kept. Names and layouts are
// placeholders.
#include "base/UnkCloneRecords.h"
#include "base/UnkVector.h"
#include "types.h"

template <class V>
struct UnkNavLoadMap;
struct UnkNavLoadSlot360;
struct UnkClone_fn_710027ead8;
extern template void base::UnkKeyedVecArray<UnkClone_fn_710027ead8>::Clear();

namespace navmesh_load_a1_b76 {

struct UnkCellRecordMap {
    void* values;
    void* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) ~UnkCellRecordMap() {
        reinterpret_cast<base::UnkKeyedVecArray<UnkClone_fn_710027ead8>*>(this)->Clear();
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};
static_assert(sizeof(UnkCellRecordMap) == 0x28, "UnkCellRecordMap layout");

struct UnkCellRecordMaps {
    UnkCellRecordMap* values;
    base::UnkStrIdMember* keys;
    u32 count;
};

}  // namespace navmesh_load_a1_b76

void fn_710027e818(UnkNavLoadMap<UnkNavLoadMap<base::UnkVector<UnkNavLoadSlot360>>>* self) {
    using base::UnkStrIdMember;
    using navmesh_load_a1_b76::UnkCellRecordMap;
    auto* map = reinterpret_cast<navmesh_load_a1_b76::UnkCellRecordMaps*>(self);
    if (map->keys && map->count)
        for (u64 i = 0; i < map->count; ++i)
            map->keys[i].~UnkStrIdMember();
    if (map->values && map->count)
        for (u32 i = 0; i < map->count; ++i)
            map->values[i].~UnkCellRecordMap();
    map->count = 0;
}
