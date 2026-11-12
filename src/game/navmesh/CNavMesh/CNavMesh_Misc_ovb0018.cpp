// navmesh::CNavMesh (tools/tu_map.py unit 355): the constructor, the cell/item/edge array helpers
// (reserve, grow, copy constructors), the per-cell link lists at +0x3b0, the box and corner-walk
// queries, and the tables' clears. Each function sees the mesh through a placeholder layout of its
// own (see src/game/navmesh/CNavMesh.cpp for the shared picture). Names and layouts are placeholders.
#include <new>
#include "UnkClone.h"
#include "base/UnkEmptyDtorElem.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/CGameObject.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU367.h"
#include "types.h"

struct UnkLinkGroups;  // the member at +0x10 (src/game/navmesh/CNavMesh/fn_710027514c.cpp)
void fn_710027497c(UnkLinkGroups* self);
extern char g_unk_7101a20e58[];  // navmesh::CNavMesh's vtable

struct UnkNavLoadCell;
void fn_7100279600(base::UnkVector<UnkNavLoadCell>* self, u32 capacity);

// A map of +0x2e8 to +0x388: the values, the keys, the count and capacity, the growth step and the
// allocator's flags.
struct UnkNavMeshMap47f8 {
    void* values;
    void* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkNavMeshMap47f8() {
        values = nullptr;
        keys = nullptr;
        count = 0;
        capacity = 0;
        step = 10;
        flags = 0x57;
        allocator = fn_7100083990();
    }
};

struct UnkNavMesh47f8 {
    void** vtable;
    u8 pad_0x8[0x8];
    u8 field_0x10[0x280];
    base::UnkVector<UnkNavLoadCell> cells;  // +0x290
    base::UnkVector<u8> vertices;           // +0x2b8
    u32 field_0x2e0;
    UnkNavMeshMap47f8 field_0x2e8;
    UnkNavMeshMap47f8 field_0x310;
    UnkNavMeshMap47f8 field_0x338;
    UnkNavMeshMap47f8 field_0x360;
    UnkNavMeshMap47f8 field_0x388;
    base::UnkVector<u8> field_0x3b0;
    base::UnkVector<u8> items;  // +0x3d8
    base::UnkVector<u8> tree;   // +0x400
};
static_assert(__builtin_offsetof(UnkNavMesh47f8, field_0x2e8) == 0x2e8, "UnkNavMesh47f8 layout");
static_assert(__builtin_offsetof(UnkNavMesh47f8, tree) == 0x400, "UnkNavMesh47f8 layout");

// navmesh::CNavMesh's constructor.
void fn_71002747f8(UnkNavMesh47f8* self) {
    fn_7101043904(self);
    self->vtable = reinterpret_cast<void**>(g_unk_7101a20e58 + 0x10);
    fn_710027497c(reinterpret_cast<UnkLinkGroups*>(self->field_0x10));
    new (&self->cells) base::UnkVector<UnkNavLoadCell>(0x57);
    fn_7100279600(&self->cells, 0);
    new (&self->vertices) base::UnkVector<u8>(0x57);
    self->field_0x2e0 = 0;
    new (&self->field_0x2e8) UnkNavMeshMap47f8();
    new (&self->field_0x310) UnkNavMeshMap47f8();
    new (&self->field_0x338) UnkNavMeshMap47f8();
    new (&self->field_0x360) UnkNavMeshMap47f8();
    new (&self->field_0x388) UnkNavMeshMap47f8();
    new (&self->field_0x3b0) base::UnkVector<u8>(0x57);
    new (&self->items) base::UnkVector<u8>(0x57);
    new (&self->tree) base::UnkVector<u8>(0x57);
}

struct UnkOwner;

void fn_71002af96c(UnkNavMeshCell* cell);
void fn_71002b3828(UnkOwner* self);

// The registry fn_71009c0ba4 returns: an array of entries, each holding the object to refresh at +0x10.
struct UnkNavRefreshEntry {
    u8 pad_0x0[0x10];
    void* object;
};
struct UnkNavRefreshList {
    UnkNavRefreshEntry** data;
    u32 count;
    UnkNavRefreshEntry** begin() const { return data; }
    UnkNavRefreshEntry** end() const { return data + count; }
};
UnkNavRefreshList* fn_71009c0ba4();
void fn_71009c1180(void* object);

struct UnkNavMeshCellSlotA24c {
    UnkNavMeshCell cell;
    u8 pad[0x70 - sizeof(UnkNavMeshCell)];
};

struct UnkNavMeshRecordA24c {
    u8 data[0x208];
};

struct UnkNavMeshA24c {
    u8 pad_0x0[0x290];
    UnkNavMeshCellSlotA24c* cells;  // +0x290
    u32 cell_count;                 // +0x298
    u8 pad_0x29c[0x3d8 - 0x29c];
    UnkNavMeshRecordA24c* records;  // +0x3d8
    u32 record_count;               // +0x3e0
    UnkNavMeshRecordA24c* begin() const { return records; }
    UnkNavMeshRecordA24c* end() const { return records + record_count; }
};

void fn_710027a24c(UnkNavMesh* navmesh) {
    auto* mesh = reinterpret_cast<UnkNavMeshA24c*>(navmesh);
    for (u16 i = 0; i < mesh->cell_count; ++i)
        fn_71002af96c(&mesh->cells[i].cell);
    for (UnkNavMeshRecordA24c& record : *mesh)
        fn_71002b3828(reinterpret_cast<UnkOwner*>(&record));
    for (UnkNavRefreshEntry* entry : *fn_71009c0ba4())
        fn_71009c1180(entry ? entry->object : nullptr);
}

using base::math::CVector2D;

struct UnkNavMeshVerticesA710 {
    u8 pad_0x0[0x2b8];
    CVector2D* vertices;  // +0x2b8
};

// An axis-aligned box: the low corner, then the high one.
struct UnkNavBoxA710 {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
};

static inline bool Near(f32 a, f32 b) { return a - 0.001f <= b && a + 0.001f >= b; }

// Whether a vertex lies in the box (with a small tolerance on each side).
static inline bool InBox(CVector2D v, const UnkNavBoxA710* box) {
    return (v.x >= box->min_x || Near(v.x, box->min_x)) && (v.x <= box->max_x || Near(v.x, box->max_x)) &&
           (v.y >= box->min_y || Near(v.y, box->min_y)) && (v.y <= box->max_y || Near(v.y, box->max_y));
}

// Whether all of the cell's vertices (`all`) or any of them lie in the box.
bool fn_710027a710(UnkNavMesh* navmesh, const UnkNavBoxA710* box, UnkNavMeshCell* cell, bool all) {
    auto* mesh = reinterpret_cast<UnkNavMeshVerticesA710*>(navmesh);
    bool result = all;
    for (u8 i = 0; i < cell->field_0x10; ++i) {
        UnkNavMeshSidePair side = fn_71002afe60(cell, &i);
        u16 vertex = side.a;
        bool in = InBox(mesh->vertices[vertex], box);
        if (all) {
            if (!in) {
                result = false;
                break;
            }
        } else if (in) {
            result = true;
            break;
        }
    }
    return result;
}

using base::math::CVector2D;

struct UnkNavMeshCellSlotCc98 {
    UnkNavMeshCell cell;
    u8 pad[0x70 - sizeof(UnkNavMeshCell)];
};

struct UnkNavMeshCellsCc98 {
    u8 pad_0x0[0x290];
    UnkNavMeshCellSlotCc98* cells;  // +0x290
};

bool fn_710027c460(UnkNavMesh* self, u16 cell, u8 side, bool forward, const CVector2D* widths, u32 flags,
                   u16* out_cell, u8* out_side);

bool fn_710027cc98(UnkNavMesh* self, u16 cell, u8 side, bool forward, const CVector2D* widths, f32 limit, u32 flags,
                   u16* out_cell, u8* out_side) {
    u16 found_cell = 0xffff;
    u8 found_side = 0xff;
    if (!fn_710027c460(self, cell, side, forward, widths, flags, &found_cell, &found_side))
        return false;
    u16 c = found_cell;
    auto* mesh = reinterpret_cast<UnkNavMeshCellsCc98*>(self);
    const UnkNavMeshEdge* edge = fn_71002afc34(&mesh->cells[c].cell, &found_side);
    if (!(edge->field_0x10 * -0.0f - edge->field_0xc <= limit))
        return false;
    *out_cell = c;
    *out_side = found_side;
    return true;
}

// The two widths the walks take (passed by value: its copy is out of line in the callee's view).
struct UnkNavWidthsCd40 {
    f32 x;
    f32 y;
    UnkNavWidthsCd40(const UnkNavWidthsCd40& o) : x(o.x), y(o.y) {}
};

struct UnkNavMeshEdgeCd40 {
    u8 index;
    u8 pad_0x1;
    u16 neighbour;      // +0x2
    u8 neighbour_side;  // +0x4
};

struct UnkNavMeshCellSlotCd40 {
    UnkNavMeshCell cell;
    u8 pad[0x70 - sizeof(UnkNavMeshCell)];
};

struct UnkNavMeshCd40 {
    u8 pad_0x0[0x290];
    UnkNavMeshCellSlotCd40* cells;  // +0x290
};

bool fn_710027ce50(UnkNavMesh* self, u16 cell, u8 side, bool forward, UnkNavWidthsCd40 widths, u32 flags,
                   f32 limit, u16* out_cell, u8* out_side, const u16& start_cell, const u8& start_side);

bool fn_710027cd40(UnkNavMesh* self, u16 cell, u8 side, bool forward, const UnkNavWidthsCd40* widths, f32 limit,
                   u32 flags, u16* out_cell, u8* out_side, bool* out_forward) {
    u16 start_cell = cell;
    u8 start_side = side;
    *out_forward = forward;
    if (fn_710027ce50(self, cell, side, forward, *widths, flags, limit, out_cell, out_side, start_cell, start_side))
        return true;
    auto* mesh = reinterpret_cast<UnkNavMeshCd40*>(self);
    auto* edge = reinterpret_cast<const UnkNavMeshEdgeCd40*>(fn_71002afc34(&mesh->cells[cell].cell, &start_side));
    bool back = !forward;
    if (fn_710027ce50(self, edge->neighbour, edge->neighbour_side, back, *widths, flags, limit, out_cell, out_side,
                      edge->neighbour, edge->neighbour_side)) {
        *out_forward = back;
        return true;
    }
    return false;
}

struct UnkNode;

// The mesh's per-cell link lists (the element accessor is a shared out-of-line copy, src/clones/).
struct UnkClone_fn_71002797bc;
using UnkNavLinkSlotE02c = UnkCloneElem<UnkClone_fn_71002797bc, 8>;
extern template UnkNavLinkSlotE02c& base::UnkVector<UnkNavLinkSlotE02c>::GrowAt(u32);

using UnkNavLinkListE02c = base::UnkVector<UnkNode*>;

// A link: the cell it sits in at +0xa.
struct UnkNavLinkE02c {
    u8 pad_0x0[0xa];
    u16 cell;
};

struct UnkNavMeshLinksE02c {
    u8 pad_0x0[0x3b0];
    base::UnkVector<UnkNavLinkSlotE02c> links;  // +0x3b0, per cell
};

static inline UnkNavLinkListE02c*& ListAt(UnkNavMeshLinksE02c* mesh, u32 cell) {
    return *reinterpret_cast<UnkNavLinkListE02c**>(mesh->links.GrowAt(cell).data);
}

// Adds `link` to the list of the cell it sits in, creating the list on first use.
void fn_710027e02c(UnkNavMesh* navmesh, UnkNode* link) {
    u16 cell = reinterpret_cast<UnkNavLinkE02c*>(link)->cell;
    if (cell == 0xffff)
        return;
    auto* mesh = reinterpret_cast<UnkNavMeshLinksE02c*>(navmesh);
    UnkNavLinkListE02c* list = ListAt(mesh, cell);
    if (!list) {
        ListAt(mesh, cell) = new (fn_71000839a0(0x28, 0x57)) UnkNavLinkListE02c(0xf);
        list = ListAt(mesh, cell);
    }
    if (list)
        list->PushBack(link);
}

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkNavKeyE5a0 {
    u32 field_0x0;
    ~UnkNavKeyE5a0() { fn_71002874dc(this); }
};

// Two parallel tables of `count` entries: key lists and string ids.
struct UnkNavTablesE5a0 {
    base::UnkVector<UnkNavKeyE5a0>* lists;
    UnkStringData** names;
    u32 count;
};

// Drops a string id's reference and points it at the pool's empty string.
static inline void ResetName(UnkNavTablesE5a0* self, u64 i) {
    UnkStringPool* pool = g_unk_7101d47108;
    if (pool) {
        UnkStringData*& data = self->names[i];
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

// Empties both tables, keeping their storage.
void fn_710027e5a0(UnkNavTablesE5a0* self) {
    if (self->names) {
        for (u64 i = 0; i < self->count; ++i)
            ResetName(self, i);
    }
    if (self->lists) {
        for (u64 i = 0; i < self->count; ++i)
            self->lists[i].Free();
    }
    self->count = 0;
}

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkNavLoadRecordMaps;  // the bucket, as src/game/navmesh/CNavMesh.cpp names it
void fn_710027f8ac(UnkNavLoadRecordMaps* self);

// A bucket (0x28 bytes): two arrays from its allocator, emptied by fn_710027f8ac before they are freed.
struct UnkNavBucketE93c {
    void* field_0x0;
    void* field_0x8;
    u8 pad_0x10[0x10];
    base::UnkAllocator* allocator;  // +0x20

    ~UnkNavBucketE93c() {
        fn_710027f8ac(reinterpret_cast<UnkNavLoadRecordMaps*>(this));
        if (field_0x8)
            allocator->vf_0x48(field_0x8);
        if (field_0x0)
            allocator->vf_0x48(field_0x0);
    }
};

// Two parallel tables of `count` entries: key lists and string ids.
struct UnkNavTablesE93c {
    base::UnkVector<UnkNavBucketE93c>* lists;
    UnkStringData** names;
    u32 count;
};

// Drops a string id's reference and points it at the pool's empty string.
static inline void ResetName(UnkNavTablesE93c* self, u64 i) {
    UnkStringPool* pool = g_unk_7101d47108;
    if (pool) {
        UnkStringData*& data = self->names[i];
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

// Empties both tables, keeping their storage.
void fn_710027e93c(UnkNavTablesE93c* self) {
    if (self->names) {
        for (u64 i = 0; i < self->count; ++i)
            ResetName(self, i);
    }
    if (self->lists) {
        for (u64 i = 0; i < self->count; ++i)
            self->lists[i].Free();
    }
    self->count = 0;
}

struct UnkNavLoadCell;
void fn_710027ebe4(base::UnkVector<UnkNavMeshEdge>* self, const base::UnkVector<UnkNavMeshEdge>* other);

struct UnkNavLoadItem;

// A cell record of the mesh's array (0x70 bytes), as src/game/navmesh/CNavMesh.cpp lays it out: moved
// by copying (the edge array by fn_710027ebe4) and destroyed out of line (fn_71002af6a0).
struct UnkNavCellRecord9600 {
    u16 id;
    alignas(8) u8 vertices_raw[0x28];  // +0x8, a base::UnkVector<u16>
    alignas(8) u8 edges_raw[0x28];     // +0x30, a base::UnkVector<UnkNavMeshEdge>
    u64 field_0x58;
    u64 field_0x60;
    UnkNavLoadItem* item;  // +0x68

    base::UnkVector<u16>& Vertices() { return *reinterpret_cast<base::UnkVector<u16>*>(vertices_raw); }
    base::UnkVector<UnkNavMeshEdge>& Edges() { return *reinterpret_cast<base::UnkVector<UnkNavMeshEdge>*>(edges_raw); }

    __attribute__((always_inline)) UnkNavCellRecord9600(UnkNavCellRecord9600&& src) {
        base::UnkVector<u16>& vertices = Vertices();
        base::UnkVector<u16>& other = src.Vertices();
        id = src.id;
        vertices.data = nullptr;
        vertices.count = 0;
        vertices.capacity = 0;
        vertices.step = other.step;
        vertices.field_0x14 = other.field_0x14;
        vertices.flags = other.flags;
        vertices.allocator = other.allocator;
        vertices.Reserve(other.capacity);
        __builtin_memcpy(vertices.data, other.data, (u64)other.count * sizeof(u16));
        vertices.count = other.count;
        fn_710027ebe4(&Edges(), &src.Edges());
        // The last three words are copied as one block.
        __builtin_memcpy(&field_0x58, &src.field_0x58, 0x18);
    }
    ~UnkNavCellRecord9600() { fn_71002af6a0(reinterpret_cast<UnkNavMeshCell*>(this)); }
};
static_assert(sizeof(UnkNavCellRecord9600) == 0x70, "UnkNavCellRecord9600 layout");

// The cell array's reserve: the cells are moved into the new array one by one.
void fn_7100279600(base::UnkVector<UnkNavLoadCell>* self, u32 capacity) {
    auto* v = reinterpret_cast<base::UnkVector<UnkNavCellRecord9600>*>(self);
    if (v->capacity < capacity) {
        auto* p = static_cast<UnkNavCellRecord9600*>(
            v->allocator->vf_0x40((u64)capacity * sizeof(UnkNavCellRecord9600), 8, v->flags));
        if (v->data) {
            UnkNavCellRecord9600* src = v->data;
            UnkNavCellRecord9600* dst = p;
            for (u32 i = 0; i < v->count; ++i, ++dst, ++src) {
                new (dst) UnkNavCellRecord9600(static_cast<UnkNavCellRecord9600&&>(*src));
                src->~UnkNavCellRecord9600();
            }
            v->allocator->vf_0x48(v->data);
        }
        v->data = p;
        v->capacity = capacity;
    }
}

struct UnkNavMeshItem;  // src/game/navmesh/UnkNavMeshItem.cpp

struct UnkRecord;  // the item, as src/game_s2/fn_71002b2e9c.cpp names it
void fn_71002b2e9c(UnkRecord* self);
void fn_71002b3114(UnkNavMeshItem* self, UnkNavMeshItem* other);
void fn_71002b3328(UnkNavMeshItem* self);

// An item (0x208 bytes) with its out-of-line constructor, move and destructor.
struct alignas(8) UnkNavItem9dd4 {
    u8 raw[0x208];

    UnkNavItem9dd4() { fn_71002b2e9c(reinterpret_cast<UnkRecord*>(this)); }
    UnkNavItem9dd4(UnkNavItem9dd4&& other) { fn_71002b3114(Get(), other.Get()); }
    ~UnkNavItem9dd4() { fn_71002b3328(Get()); }

    UnkNavMeshItem* Get() { return reinterpret_cast<UnkNavMeshItem*>(this); }
};

// The item array's element accessor (it grows the array to cover the index).
__attribute__((flatten)) UnkNavMeshItem* fn_7100279dd4(base::UnkVector<UnkNavMeshItem>* self, u32 index) {
    return reinterpret_cast<base::UnkVector<UnkNavItem9dd4>*>(self)->GrowAt(index).Get();
}

// An edge record (game/UnkNavMeshCell.h) with the copy the cell copies use: the side and neighbour
// word, three words and the four links (the cells and edges the walks round the side's corners end on).
struct UnkNavEdgeCopyEbe4 {
    struct Head {
        u8 index;
        u8 pad_0x1;
        u16 neighbour;
        u8 neighbour_side;
        u8 pad_0x5[3];
        f32 width;
    } head;
    u32 field_0xc;
    u32 field_0x10;
    struct Links {
        void* field_0x18;
        void* field_0x20;
        void* field_0x28;
        void* field_0x30;
    } links;

    UnkNavEdgeCopyEbe4(const UnkNavEdgeCopyEbe4& o)
        : head(o.head), field_0xc(o.field_0xc), field_0x10(o.field_0x10), links(o.links) {}
};
static_assert(sizeof(UnkNavEdgeCopyEbe4) == sizeof(UnkNavMeshEdge), "UnkNavEdgeCopyEbe4 layout");

// The edge array's copy constructor: the other's settings, its capacity, then its edges one by one.
__attribute__((flatten)) void fn_710027ebe4(base::UnkVector<UnkNavMeshEdge>* self, const base::UnkVector<UnkNavMeshEdge>* other) {
    auto* dst = reinterpret_cast<base::UnkVector<UnkNavEdgeCopyEbe4>*>(self);
    auto* src = reinterpret_cast<const base::UnkVector<UnkNavEdgeCopyEbe4>*>(other);
    dst->data = nullptr;
    dst->count = 0;
    dst->capacity = 0;
    dst->step = src->step;
    dst->field_0x14 = src->field_0x14;
    dst->flags = src->flags;
    dst->allocator = src->allocator;
    dst->Reserve(src->capacity);
    for (const UnkNavEdgeCopyEbe4* e = src->data, *end = src->data + src->count; e != end; ++e)
        dst->PushBackCopy(*e);
}

// A 4-byte key of a link's key array, copied member-wise and destroyed out of line.
struct UnkNavLinkKeyE258 {
    u32 value;
    UnkNavLinkKeyE258(const UnkNavLinkKeyE258& o) { __builtin_memcpy(&value, &o.value, sizeof(value)); }
    ~UnkNavLinkKeyE258() { fn_71002874dc(this); }
};

// The key array's copy constructor: the other's settings, its capacity, then its keys one by one.
__attribute__((flatten)) void fn_710027e258(void* self, const void* other) {
    auto* dst = static_cast<base::UnkVector<UnkNavLinkKeyE258>*>(self);
    auto* src = static_cast<const base::UnkVector<UnkNavLinkKeyE258>*>(other);
    dst->data = nullptr;
    dst->count = 0;
    dst->capacity = 0;
    dst->step = src->step;
    dst->field_0x14 = src->field_0x14;
    dst->flags = src->flags;
    dst->allocator = src->allocator;
    dst->Reserve(src->capacity);
    for (const UnkNavLinkKeyE258* e = src->data, *end = src->data + src->count; e != end; ++e)
        dst->PushBackCopy(*e);
}
