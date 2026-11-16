// navmesh::CNavMesh: the per-side visitor of fn_710027bbf0's nearest-side search. A side that the
// callbacks accept, that is open for the flags and nearer to the point than the best so far
// becomes the best. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkTU362.h"
#include "types.h"

using base::math::CVector2D;

// A cell record of the mesh's array (the header spells out only the members the lookups use).
struct UnkNavMeshCellSlot {
    UnkNavMeshCell cell;
    u8 pad[0x70 - sizeof(UnkNavMeshCell)];
};
static_assert(sizeof(UnkNavMeshCellSlot) == 0x70, "UnkNavMeshCellSlot layout");

struct UnkNavMeshTreeNode;

// The mesh, as far as this file reads it.
struct UnkNavMeshLayout {
    u8 pad_0x0[0x290];
    UnkNavMeshCellSlot* cells;  // +0x290
    u32 cell_count;             // +0x298
    u8 pad_0x29c[0x400 - 0x29c];
    UnkNavMeshTreeNode* nodes;  // +0x400, the bounding-box tree (src/game/navmesh/CNavMesh/fn_710027a41c.cpp)
    u32 node_count;             // +0x408
};

// The best side found so far: the cell, the side and the closest point on it. Its constructor
// (fn_7100287598) and destructor (fn_710028760c, empty) are out of line; it is copied word by word.
struct UnkNavMeshHit;
struct UnkElem_7100286624;  // the same 12-byte record (src/game_lo/fn_7100286624.cpp)

struct UnkNavMeshHit {
    u16 cell;
    u8 side;
    CVector2D point;

    UnkNavMeshHit() { fn_7100287598(this); }
    UnkNavMeshHit(const UnkNavMeshHit& other) {
        __builtin_memcpy(static_cast<void*>(&cell), &other.cell, 4);  // the cell, the side and the pad
        point.x = other.point.x;
        point.y = other.point.y;
    }
    ~UnkNavMeshHit() { fn_710028760c(reinterpret_cast<UnkElem_7100286624*>(this)); }

    bool IsValid() const { return cell != 0xffff && side != 0xff; }
};
static_assert(sizeof(UnkNavMeshHit) == 0xc, "UnkNavMeshHit layout");

struct UnkDirectionFilter;  // src/game/UnkDirectionFilter.cpp

// The closure of the per-side visitor fn_710027c298 (a lambda of fn_710027bbf0, capturing by
// reference): it measures a side of a cell against the best one so far.
struct UnkNavMeshVisitor {
    UnkNavMesh* mesh;
    f32* best_distance;
    UnkNavMeshHit* best;
    const CVector2D* point;
    u32* flags;
    const void* field_0x28;  // fn_710027bbf0's stack arguments (the callbacks)
    const void* field_0x30;
    const void* field_0x38;
    UnkDirectionFilter* direction;
    u64* filter_arg;
    u32* filter_arg2;
    u32* filter_arg3;
};

// The callbacks: a context and a function taking it first.
struct UnkPointSideFilter_27c298 {
    void* context;
    bool (*call)(void* context, const CVector2D* point, u16 cell, u8 side, void* a, void* b, u64 arg, u32 arg2,
                 u32 arg3);
    bool operator()(const CVector2D* point, u16 cell, u8 side, void* a, void* b, u64 arg, u32 arg2, u32 arg3) const {
        return call(context, point, cell, side, a, b, arg, arg2, arg3);
    }
};
struct UnkSideFilter_27c298 {
    void* context;
    bool (*call)(void* context, u16 cell, u8 side, void* a, void* b, u64 arg, u32 arg2, u32 arg3);
    bool operator()(u16 cell, u8 side, void* a, void* b, u64 arg, u32 arg2, u32 arg3) const {
        return call(context, cell, side, a, b, arg, arg2, arg3);
    }
};
struct UnkSegmentFilter_27c298 {
    void* context;
    s32 (*call)(void* context, const UnkNavMeshSideSegment* segment);
    s32 operator()(const UnkNavMeshSideSegment* segment) const { return call(context, segment); }
};

struct UnkSegment2D_71001262e0;
f32 fn_71001262e0(const UnkSegment2D_71001262e0* seg, const CVector2D* p, CVector2D* closest, f32* t);
struct UnkSegment2D;
bool fn_71006f379c(const UnkDirectionFilter* self, const UnkSegment2D* seg);
bool fn_71002afc84(UnkNavMeshCell* cell, const u8* index, u32 flags);

static inline UnkNavMeshCell* CellAt(UnkNavMesh* mesh, u16 cell) {
    return &reinterpret_cast<UnkNavMeshLayout*>(mesh)->cells[cell].cell;
}

void fn_710027c298(UnkNavMeshVisitor* self, u16 cell, u8 side) {
    UnkNavMesh* mesh = self->mesh;
    const UnkPointSideFilter_27c298* pointFilter = static_cast<const UnkPointSideFilter_27c298*>(self->field_0x28);
    if (pointFilter->call) {
        if (!(*pointFilter)(self->point, cell, side, nullptr, nullptr, *self->filter_arg,
                               *self->filter_arg2, *self->filter_arg3))
            return;
    } else {
        const UnkSideFilter_27c298* sideFilter = static_cast<const UnkSideFilter_27c298*>(self->field_0x30);
        if (sideFilter->call && !(*sideFilter)(cell, side, nullptr, nullptr, *self->filter_arg,
                                                  *self->filter_arg2, *self->filter_arg3))
            return;
    }
    if (fn_71002afc84(CellAt(mesh, cell), &side, *self->flags))
        return;

    UnkNavMeshSideSegment segment = {{0.0f, 0.0f}, {0.0f, 0.0f}, -1.0f};
    fn_71002afea8(CellAt(mesh, cell), &side, &segment);
    CVector2D closest = {0.0f, 0.0f};
    f32 distSq = fn_71001262e0(reinterpret_cast<const UnkSegment2D_71001262e0*>(&segment), self->point, &closest,
                               nullptr);
    if (!(distSq < *self->best_distance * *self->best_distance))
        return;

    const UnkSegmentFilter_27c298* segmentFilter = static_cast<const UnkSegmentFilter_27c298*>(self->field_0x38);
    bool accepted;
    if (segmentFilter->call && (*segmentFilter)(&segment))
        accepted = (*static_cast<const UnkSegmentFilter_27c298*>(self->field_0x38))(&segment) == 1;
    else
        accepted = fn_71006f379c(self->direction, reinterpret_cast<const UnkSegment2D*>(&segment));
    if (!accepted)
        return;

    *self->best_distance = sqrtf(distSq);
    self->best->cell = cell;
    self->best->side = side;
    self->best->point = closest;
}
