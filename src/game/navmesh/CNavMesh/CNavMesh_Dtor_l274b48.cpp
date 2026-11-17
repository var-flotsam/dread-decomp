// navmesh::CNavMesh (tools/tu_map.py unit 355): the destructor. After the mesh's own clean-up
// (fn_7100274df0) it frees the arrays and tables the constructor (fn_71002747f8, CNavMesh_Misc_ovb0018.cpp)
// builds, last to first, then the link groups at +0x10 and the game object base. Names and layouts are
// placeholders.
#include "base/UnkCloneRecords.h"
#include "base/global/CRntVector.h"
#include "game/CGameObject.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkTU107.h"
#include "game/UnkTU367.h"
#include "game/UnkTU372.h"
#include "game/navmesh/CNavMesh.h"
#include "types.h"

struct UnkLinkGroups;
void fn_710027514c(UnkLinkGroups* self);

// The table clears (CNavMesh_Misc_ovb0018.cpp, UnkNavLoadMapClear_A1_b76.cpp, src/clones/).
struct UnkNavTablesE5a0;
struct UnkNavTablesE93c;
template <class V>
struct UnkNavLoadMap;
struct UnkNavLoadSlot360;
void fn_710027e5a0(UnkNavTablesE5a0* self);
void fn_710027e93c(UnkNavTablesE93c* self);
void fn_710027e818(UnkNavLoadMap<UnkNavLoadMap<base::global::CRntVector<UnkNavLoadSlot360>>>* self);
struct UnkClone_fn_710027e494;
struct UnkClone_fn_710027e70c;
extern template void base::UnkKeyedVecArray<UnkClone_fn_710027e494>::Clear();
extern template void base::UnkKeyedVecArray<UnkClone_fn_710027e70c>::Clear();

using UnkNavLoadMaps_l274b48 = UnkNavLoadMap<UnkNavLoadMap<base::global::CRntVector<UnkNavLoadSlot360>>>;

static inline void ClearE494(base::UnkKeyedVecArray<UnkClone_fn_710027e494>* self) {
    self->Clear();
}
static inline void ClearE70c(base::UnkKeyedVecArray<UnkClone_fn_710027e70c>* self) {
    self->Clear();
}

// One of the mesh's tables: the values, the keys, the count and capacity, the growth step, the
// allocator's flags and the allocator. Destroyed by its clear, then freeing the keys and the values.
template <class Tables, void (*Clear)(Tables*)>
struct UnkNavTable_l274b48 {
    void* values;
    void* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    ~UnkNavTable_l274b48() {
        Clear(reinterpret_cast<Tables*>(this));
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};

// A cell of the array at +0x290 (0x70 bytes).
struct UnkNavCellSlot_l274b48 {
    UnkNavMeshCell cell;
    u8 pad[0x70 - sizeof(UnkNavMeshCell)];

    ~UnkNavCellSlot_l274b48() { fn_71002af6a0(&cell); }
};
static_assert(sizeof(UnkNavCellSlot_l274b48) == 0x70, "UnkNavCellSlot_l274b48 layout");

// An item of the array at +0x3d8 (0x208 bytes).
struct UnkNavItemSlot_l274b48 {
    u8 data[0x208];

    ~UnkNavItemSlot_l274b48() { fn_71002b3328(reinterpret_cast<UnkNavMeshItem*>(data)); }
};

struct UnkNavMesh_l274b48 {
    void** vtable;
    u8 pad_0x8[0x8];
    u8 field_0x10[0x280];  // the link groups
    base::global::CRntVector<UnkNavCellSlot_l274b48> cells;  // 0x290
    base::global::CRntVector<u8> vertices;  // 0x2b8
    u32 field_0x2e0;
    UnkNavTable_l274b48<base::UnkKeyedVecArray<UnkClone_fn_710027e494>, ClearE494> field_0x2e8;
    UnkNavTable_l274b48<UnkNavTablesE5a0, fn_710027e5a0> field_0x310;
    UnkNavTable_l274b48<base::UnkKeyedVecArray<UnkClone_fn_710027e70c>, ClearE70c> field_0x338;
    UnkNavTable_l274b48<UnkNavLoadMaps_l274b48, fn_710027e818> field_0x360;
    UnkNavTable_l274b48<UnkNavTablesE93c, fn_710027e93c> field_0x388;
    base::global::CRntVector<u8> field_0x3b0;
    base::global::CRntVector<UnkNavItemSlot_l274b48> items;  // 0x3d8
    base::global::CRntVector<u8> tree;  // 0x400
};
static_assert(__builtin_offsetof(UnkNavMesh_l274b48, field_0x2e8) == 0x2e8, "UnkNavMesh_l274b48 layout");
static_assert(__builtin_offsetof(UnkNavMesh_l274b48, tree) == 0x400, "UnkNavMesh_l274b48 layout");

void fn_7100274df0(UnkNavMesh_l274b48* self);

void fn_7100274b48(UnkNavMesh_l274b48* self) {
    self->vtable = reinterpret_cast<void**>(g_unk_7101a20e58 + 0x10);
    fn_7100274df0(self);
    fn_710010d9f4(&self->tree);
    self->tree.Free();
    self->items.Free();
    self->field_0x3b0.Free();
    self->field_0x388.~UnkNavTable_l274b48();
    self->field_0x360.~UnkNavTable_l274b48();
    self->field_0x338.~UnkNavTable_l274b48();
    self->field_0x310.~UnkNavTable_l274b48();
    self->field_0x2e8.~UnkNavTable_l274b48();
    self->vertices.Free();
    self->cells.Free();
    fn_710027514c(reinterpret_cast<UnkLinkGroups*>(self->field_0x10));
    fn_7101043934(reinterpret_cast<UnkCameraCtrl_ovb0049*>(self));
}
