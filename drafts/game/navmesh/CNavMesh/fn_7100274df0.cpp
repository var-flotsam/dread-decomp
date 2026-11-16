// navmesh::CNavMesh: empties the mesh: destroys the cells, drops the vertices, empties the cell id map and
// the bounding-box tree, frees every record array the per-cell slots of the two record maps own, empties
// those maps, the map at +0x388, the items and the per-cell link arrays, then hands the navmesh manager
// (created on first use) to fn_7100ebfa68. Names and layouts are placeholders.
#include "base/UnkCloneRecords.h"
#include "base/global/CRntVector.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkTU107.h"
#include "game/UnkTU367.h"
#include "types.h"

using base::global::CRntVector;

template <class V>
struct UnkNavLoadMap;
struct UnkNavLoadSlot360;
struct UnkNavTablesE93c;
struct UnkNavMeshItem;
struct UnkClone_fn_710027e494;
struct UnkClone_fn_710027e70c;
struct UnkClone_fn_710027ead8;
extern template void base::UnkKeyedVecArray<UnkClone_fn_710027e494>::Clear();
extern template void base::UnkKeyedVecArray<UnkClone_fn_710027e70c>::Clear();
extern template void base::UnkKeyedVecArray<UnkClone_fn_710027ead8>::Clear();
void fn_710027e818(UnkNavLoadMap<UnkNavLoadMap<base::global::CRntVector<UnkNavLoadSlot360>>>* self);
void fn_710027e93c(UnkNavTablesE93c* self);
void fn_71002b3328(UnkNavMeshItem* item);
void fn_7100ebfa68(UnkNavMeshManager* self);

// A cell record (0x70 bytes) with its out-of-line destructor.
struct UnkNavCell_l274df0 {
    UnkNavMeshCell cell;
    u8 pad[0x70 - sizeof(UnkNavMeshCell)];

    ~UnkNavCell_l274df0() { fn_71002af6a0(&cell); }
};

struct UnkNavVertex_l274df0 {
    f32 x;
    f32 y;
};

// An item (0x208 bytes) with its out-of-line destructor.
struct UnkNavItem_l274df0 {
    u64 raw[0x208 / 8];

    ~UnkNavItem_l274df0() { fn_71002b3328(reinterpret_cast<UnkNavMeshItem*>(this)); }
};

// A word and a record: the elements of the record arrays of the map at +0x360.
struct UnkNavPathRecord_l274df0 {
    u32 word;
    UnkObject_0x100 record;
};
static_assert(sizeof(UnkNavPathRecord_l274df0) == 0x108, "UnkNavPathRecord_l274df0 layout");

// A map from string ids to values (two parallel arrays).
template <class V>
struct UnkNavMap_l274df0 {
    V* values;
    void* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

// Per cell, an owned array (or none).
template <class T>
using UnkNavSlots_l274df0 = CRntVector<CRntVector<T>*>;

struct UnkNavMesh_l274df0 {
    u8 pad_0x0[0x290];
    CRntVector<UnkNavCell_l274df0> cells;                                       // +0x290
    CRntVector<UnkNavVertex_l274df0> vertices;                                  // +0x2b8
    u8 pad_0x2e0[0x8];
    UnkNavMap_l274df0<CRntVector<u8>> field_0x2e8;                              // cell id lists
    UnkNavMap_l274df0<CRntVector<u8>> field_0x310;
    UnkNavMap_l274df0<UnkNavSlots_l274df0<UnkObject_0x100>> field_0x338;
    UnkNavMap_l274df0<UnkNavMap_l274df0<UnkNavSlots_l274df0<UnkNavPathRecord_l274df0>>> field_0x360;
    UnkNavMap_l274df0<CRntVector<u8>> field_0x388;
    UnkNavSlots_l274df0<u8> field_0x3b0;                                        // per cell
    CRntVector<UnkNavItem_l274df0> items;                                       // +0x3d8
    u8 tree[0x8];                                                               // +0x400
};
static_assert(__builtin_offsetof(UnkNavMesh_l274df0, field_0x338) == 0x338, "UnkNavMesh_l274df0 layout");
static_assert(__builtin_offsetof(UnkNavMesh_l274df0, items) == 0x3d8, "UnkNavMesh_l274df0 layout");
static_assert(__builtin_offsetof(UnkNavMesh_l274df0, tree) == 0x400, "UnkNavMesh_l274df0 layout");

template <class T>
static inline void DeleteArray(CRntVector<T>* array) {
    if (array) {
        array->Free();
        delete array;
    }
}

// Deletes the arrays the slots own and empties the slots.
template <class T>
static inline void ClearSlots(UnkNavSlots_l274df0<T>* slots) {
    for (CRntVector<T>** p = slots->data; p != slots->data + slots->count; ++p)
        DeleteArray(*p);
    slots->count = 0;
}

// Empties the slots of every value of a map.
template <class T>
static inline void ClearAllSlots(UnkNavMap_l274df0<UnkNavSlots_l274df0<T>>* map) {
    for (UnkNavSlots_l274df0<T>* slots = map->values; slots != &map->values[map->count]; ++slots)
        ClearSlots(slots);
}

template <class Tag, class V>
static inline void ClearMap(UnkNavMap_l274df0<V>* map) {
    reinterpret_cast<base::UnkKeyedVecArray<Tag>*>(map)->Clear();
}

static inline UnkNavMeshManager* GetNavMeshManager() {
    if (!g_unk_71726a4510) {
        UnkNavMeshManager* manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return g_unk_71726a4510;
}

void fn_7100274df0(UnkNavMesh_l274df0* self) {
    self->cells.Clear();
    self->vertices.Clear();
    ClearMap<UnkClone_fn_710027e494>(&self->field_0x2e8);
    fn_710010d9f4(self->tree);

    ClearAllSlots(&self->field_0x338);
    ClearMap<UnkClone_fn_710027e70c>(&self->field_0x338);

    UnkNavMap_l274df0<UnkNavMap_l274df0<UnkNavSlots_l274df0<UnkNavPathRecord_l274df0>>>* path_records =
        &self->field_0x360;
    for (UnkNavMap_l274df0<UnkNavSlots_l274df0<UnkNavPathRecord_l274df0>>* map = path_records->values;
         map != &path_records->values[path_records->count]; ++map) {
        ClearAllSlots(map);
        ClearMap<UnkClone_fn_710027ead8>(map);
    }
    fn_710027e818(reinterpret_cast<UnkNavLoadMap<UnkNavLoadMap<CRntVector<UnkNavLoadSlot360>>>*>(path_records));

    fn_710027e93c(reinterpret_cast<UnkNavTablesE93c*>(&self->field_0x388));
    self->items.Clear();

    CRntVector<u8>** links = self->field_0x3b0.data;
    u32 link_count = self->field_0x3b0.count;
    for (u32 i = 0; i < link_count; ++i)
        DeleteArray(links[i]);
    self->field_0x3b0.count = 0;

    fn_7100ebfa68(GetNavMeshManager());
}
