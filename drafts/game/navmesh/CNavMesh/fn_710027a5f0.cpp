// navmesh::CNavMesh: whether a point lies inside a cell's polygon (even-odd crossing test over the
// cell's sides, vertices at +0x2b8). Names and layouts are placeholders.
#include "game/UnkNavMeshCell.h"
#include "types.h"

struct UnkNavVertexA5f0 {
    f32 x;
    f32 y;
};

struct UnkNavPointA5f0 {
    f32 x;
    f32 y;
};

struct UnkNavMeshA5f0 {
    u8 pad_0x0[0x290];
    u8* cells;  // +0x290, 0x70 bytes each
    u32 cell_count;
    u8 pad_0x29c[0x1c];
    UnkNavVertexA5f0* vertices;  // +0x2b8
};
static_assert(__builtin_offsetof(UnkNavMeshA5f0, vertices) == 0x2b8, "UnkNavMeshA5f0 layout");

static inline bool CrossingTest(UnkNavMeshA5f0* self, UnkNavMeshCell* cell, const u8& sides,
                                const UnkNavPointA5f0* point) {
    bool inside = false;
    for (u32 i = 0; i < sides; ++i) {
        u8 side = i;
        UnkNavMeshSidePair ids = fn_71002afe60(cell, &side);
        const UnkNavVertexA5f0& a = self->vertices[ids.a];
        const UnkNavVertexA5f0& b = self->vertices[ids.b];
        f32 py = point->y;
        if ((b.y <= py && py < a.y) || (py < b.y && a.y <= py)) {
            if (point->x - b.x < (py - b.y) * (a.x - b.x) / (a.y - b.y))
                inside = !inside;
        }
    }
    return inside;
}

bool fn_710027a5f0(UnkNavMeshA5f0* self, const UnkNavPointA5f0* point, u16 cell_id) {
    if (cell_id >= self->cell_count)
        return false;
    UnkNavMeshCell* cell = reinterpret_cast<UnkNavMeshCell*>(self->cells + (u64)cell_id * 0x70);
    const u8& sides = cell->field_0x10;
    if (sides < 3)
        return false;
    return CrossingTest(self, cell, sides, point);
}
