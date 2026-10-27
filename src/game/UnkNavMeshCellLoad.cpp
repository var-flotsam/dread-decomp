// A navigation-mesh cell (include/game/UnkNavMeshCell.h), translation unit 367 of
// data/tu_map.csv, next to src/game/UnkNavMeshCell.cpp: setting a cell's edge record by side and
// reading a cell from a file. A cell has its id at +0, the vertex ids of its corners at +8 and the
// edge table at +0x30 (base::UnkVector, of 0x38-byte records; growable flag at +0x44), then the mesh
// it belongs to at +0x58.
// Holds fn_71002afa48 and fn_71002b0880. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkVector.h"
#include "base/global/CRntFile.h"
#include "base/math/CVector.h"
#include "game/UnkNavMeshCell.h"
#include "types.h"

using base::math::CVector2D;

// A point of an edge record, copied component by component.
struct UnkNavEdgePoint {
    f32 x, y;

    UnkNavEdgePoint(const CVector2D& v) : x(v.x), y(v.y) {}
    UnkNavEdgePoint(const UnkNavEdgePoint& other) : x(other.x), y(other.y) {}
    UnkNavEdgePoint& operator=(const UnkNavEdgePoint& other) {
        x = other.x;
        y = other.y;
        return *this;
    }
};

// The edge record (UnkNavMeshEdge's layout, with the members this unit sets).
struct UnkNavEdgeRecord {
    u8 index;
    u16 neighbour;
    u8 field_0x4;
    f32 field_0x8;
    UnkNavEdgePoint field_0xc;
    u64 field_0x18[4] = {};

    UnkNavEdgeRecord(UnkNavEdgePoint field_0xc_, u8 index_, u16 neighbour_, u8 field_0x4_, f32 field_0x8_)
        : index(index_), neighbour(neighbour_), field_0x4(field_0x4_), field_0x8(field_0x8_), field_0xc(field_0xc_) {}
};
static_assert(sizeof(UnkNavEdgeRecord) == 0x38, "UnkNavEdgeRecord placeholder layout");

using UnkNavEdgeVector = base::UnkVector<UnkNavEdgeRecord>;

// Resizes the table to `n` records (same unit, not decompiled yet).
void fn_71002b0e78(UnkNavEdgeVector* v, u32 n);

static inline UnkNavEdgeVector* Edges(UnkNavMeshCell* cell) {
    return reinterpret_cast<UnkNavEdgeVector*>(reinterpret_cast<char*>(cell) + 0x30);
}

// The record at `index`, growing a growable table to cover it first.
static inline UnkNavEdgeRecord& EdgeAt(UnkNavEdgeVector* v, u32 index) {
    if (v->field_0x14 && v->count <= index) {
        if (v->capacity <= index && v->step != 0)
            v->Reserve((index / v->step + 1) * v->step);
        fn_71002b0e78(v, index + 1);
    }
    return v->data[index];
}

// Sets the cell's edge record for side `index`.
void fn_71002afa48(UnkNavMeshCell* cell, const u8& index, const u16& neighbour, const u8& field_0x4, f32 value,
                   const CVector2D& point) {
    EdgeAt(Edges(cell), index) = UnkNavEdgeRecord(point, index, neighbour, field_0x4, value);
}

// The cell as fn_71002b0880 fills it.
struct Mesh;
struct Vec2;
struct UnkNavCellData {
    u16 id;
    base::UnkVector<u16> vertices;
    UnkNavEdgeVector edges;
    Mesh* mesh;
};
static_assert(__builtin_offsetof(UnkNavCellData, edges) == 0x30, "UnkNavCellData placeholder layout");
static_assert(__builtin_offsetof(UnkNavCellData, mesh) == 0x58, "UnkNavCellData placeholder layout");

// The position of vertex `id` of the mesh (src/game_hi/fn_7100ebfa7c.cpp; a CVector2D).
Vec2* fn_710027a410(Mesh*, u16);

// The record values of a side with no edge in the file: no neighbour and no flags.
extern const u16 g_unk_7101648ce8 __attribute__((visibility("hidden")));
extern const u8 g_unk_7101648cea __attribute__((visibility("hidden")));

// The vertex ids at the two ends of side `side` (0xffff for both when there is no such side).
struct UnkSideEnds {
    u16 a;
    u16 b;
};
static inline UnkSideEnds SideEnds(const UnkNavCellData* cell, const u8& side) {
    UnkSideEnds ends;
    if (side == 0xff || side >= cell->vertices.count) {
        ends.a = 0xffff;
        ends.b = 0xffff;
    } else {
        u8 next = side + 1;
        if (next >= cell->vertices.count)
            next = 0;
        ends.a = cell->vertices.data[side];
        ends.b = cell->vertices.data[next];
    }
    return ends;
}

// The unit direction of side `side` of the cell and its length.
static inline f32 SideDirection(UnkNavCellData* cell, Mesh* mesh, const u8& side, CVector2D* dir) {
    UnkSideEnds ends = SideEnds(cell, side);
    const CVector2D* a = reinterpret_cast<const CVector2D*>(fn_710027a410(mesh, ends.a));
    f32 ax = a->x;
    f32 ay = a->y;
    const CVector2D* b = reinterpret_cast<const CVector2D*>(fn_710027a410(mesh, ends.b));
    f32 dx = b->x - ax;
    f32 dy = b->y - ay;
    f32 len = sqrtf(dx * dx + dy * dy);
    dir->x = dx / len;
    dir->y = dy / len;
    return len;
}

// Reads cell `id` of `mesh` from `file`: the vertex ids of its corners, then its edges (side,
// neighbour and flags each, as 32-bit values); sides without an edge in the file get one with no
// neighbour.
// NON_MATCHING(minor): instruction scheduling in the two inlined side-end lookups (the original loads
// the vertex count before masking the side and the first vertex id after computing the next side),
// with the registers renamed to suit; the same loads and branches.
void fn_71002b0880(UnkNavCellData* cell, base::global::CRntFile* file, u16 id, Mesh* mesh) {
    cell->id = id;
    cell->mesh = mesh;
    s32 n = 0;
    fn_710007c714(file, &n, 4, 1);
    if (n > 0) {
        cell->vertices.Reserve(n);
        for (s32 i = 0; i < n; ++i) {
            u32 vertex = 0xffff;
            fn_710007c714(file, &vertex, 4, 1);
            cell->vertices.PushBack(vertex);
        }
    }
    if (n > 0) {
        s32 m = 0;
        fn_710007c714(file, &m, 4, 1);
        if (m > 0) {
            cell->edges.Reserve(n);
            for (s32 i = 0; i < m; ++i) {
                u32 side = 0xff;
                u32 neighbour = 0xffff;
                u32 flags = 0xff;
                fn_710007c714(file, &side, 4, 1);
                fn_710007c714(file, &neighbour, 4, 1);
                fn_710007c714(file, &flags, 4, 1);
                CVector2D dir;
                f32 len = SideDirection(cell, mesh, side, &dir);
                fn_71002afa48(reinterpret_cast<UnkNavMeshCell*>(cell), side, neighbour, flags, len, dir);
            }
            for (u8 side = 0; side < n; ++side) {
                if (EdgeAt(&cell->edges, side).index == 0xff) {
                    CVector2D dir;
                    f32 len = SideDirection(cell, mesh, side, &dir);
                    fn_71002afa48(reinterpret_cast<UnkNavMeshCell*>(cell), side, g_unk_7101648ce8,
                                  g_unk_7101648cea, len, dir);
                }
            }
        }
    }
}
