// Unit 372 (no reliable name): turns a set of navmesh cells into a triangle list (each cell a fan
// from its first vertex) and its 2D bounds; false when the vertex limit is reached.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CRntVector;
using base::math::CVector2D;

struct UnkNavCellSet_l2b3ccc {
    u8 pad_0x0[0xc8];
    u16* cells;      // 0xc8
    u32 cell_count;  // 0xd0
};

// Side `side` of the cell as a segment (left zero, with -1 in its last word, when there is none).
static inline UnkNavMeshSideSegment GetSide(const UnkNavMeshCell* cell, const u8& side) {
    UnkNavMeshSideSegment seg = {};
    seg.field_0x10 = -1.0f;
    fn_71002afea8(cell, &side, &seg);
    return seg;
}

static inline void Expand(CVector2D* min, CVector2D* max, f32 x, f32 y) {
    if (x < min->x)
        min->x = x;
    if (y < min->y)
        min->y = y;
    if (x > max->x)
        max->x = x;
    if (y > max->y)
        max->y = y;
}

bool fn_71002b3ccc(UnkNavCellSet_l2b3ccc* self, UnkNavMesh* mesh, u32 max_vertices, CVector2D* min,
                   CVector2D* max, CRntVector<UnkVec3f>* vertices, CRntVector<u16>* indices) {
    *min = CVector2D{3.4028235e38f, 3.4028235e38f};
    *max = CVector2D{-3.4028235e38f, -3.4028235e38f};
    for (u16 c = 0; c < self->cell_count; ++c) {
        UnkNavMeshCell* cell = fn_710027a328(mesh, self->cells[c]);
        if (cell->field_0x10 < 3)
            continue;

        UnkNavMeshSideSegment first_side = GetSide(cell, 0);
        CVector2D v = first_side.start;
        if (min->x > v.x)
            min->x = v.x;
        if (min->y > v.y)
            min->y = v.y;
        if (max->x < v.x)
            max->x = v.x;
        if (max->y < v.y)
            max->y = v.y;
        vertices->EmplaceBack(UnkVec3f(v.x, v.y, 0.0f));
        if (vertices->count >= max_vertices)
            return false;
        u16 first = vertices->count - 1;

        for (u8 i = 1; i < cell->field_0x10 - 1; ++i) {
            // Read and not used in the original.
            UnkNavMeshSideSegment prev = GetSide(cell, i - 1);
            UnkNavMeshSideSegment side = GetSide(cell, i);
            Expand(min, max, side.start.x, side.start.y);
            vertices->EmplaceBack(UnkVec3f(side.start.x, side.start.y, 0.0f));
            if (vertices->count >= max_vertices)
                return false;
            Expand(min, max, side.end.x, side.end.y);
            vertices->EmplaceBack(UnkVec3f(side.end.x, side.end.y, 0.0f));
            if (vertices->count >= max_vertices)
                return false;
            indices->PushBack(first);
            indices->PushBack(vertices->count - 2);
            indices->PushBack(vertices->count - 1);
        }
    }
    return true;
}
