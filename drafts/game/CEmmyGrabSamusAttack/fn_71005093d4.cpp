// CEmmyGrabSamusAttack unit (tools/tu_map.py): finds the navmesh point at most 1000 units along the
// climbable sides from the point `t` of the way along side `*side` of cell `*cell`, walking forwards or
// backwards round the cells (fn_710027c460) while each side passes fn_71005091d4. Names and layouts
// are placeholders.
#include "base/math/CVector.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkTU363.h"
#include "types.h"

using base::math::CVector2D;

// The point found: the cell and side it is on, where, and how far it is along the walk.
struct UnkNavPoint_l5093d4 {
    u16 cell;
    u8 side;
    CVector2D pos;
    f32 distance;
};
static_assert(__builtin_offsetof(UnkNavPoint_l5093d4, distance) == 0xc, "layout");

// An edge as read here: its length and direction.
struct UnkNavEdge_l5093d4 {
    u8 pad_0x0[0x8];
    f32 length;
    CVector2D dir;
};

struct UnkNavCellSlot_l5093d4 {
    u8 data[0x70];
};

struct UnkNavMeshView_l5093d4 {
    u8 pad_0x0[0x290];
    UnkNavCellSlot_l5093d4* cells;  // +0x290

    UnkNavMeshCell* Cell(u16 id) { return reinterpret_cast<UnkNavMeshCell*>(&cells[id]); }
};

// What fn_71005091d4 tests a side with (UnkNavQuery_ovb0014 in
// src/game/CEmmyGrabSamusAttack/CEmmyGrabSamusAttack_Grab_ovb0014.cpp).
struct UnkComponent_7100286798;
struct UnkNavQuery_l5093d4 {
    UnkComponent_7100286798* component;
    UnkNavMesh* mesh;
};

struct UnkNavQuery_ovb0014;
bool fn_71005091d4(UnkNavQuery_ovb0014* query, UnkNavMeshCell* cell, const UnkNavMeshEdge* side);
bool fn_710027c460(UnkNavMesh* self, u16 cell, u8 side, bool forward, const CVector2D* widths, u32 flags,
                   u16* out_cell, u8* out_side);

static const f32 kMaxDistance = 1000.0f;

inline CVector2D Add_l5093d4(const CVector2D& a, const CVector2D& b) { return {a.x + b.x, a.y + b.y}; }
inline CVector2D Sub_l5093d4(const CVector2D& a, const CVector2D& b) { return {a.x - b.x, a.y - b.y}; }
inline CVector2D Scale_l5093d4(const CVector2D& a, f32 t) { return {a.x * t, a.y * t}; }
inline CVector2D Lerp_l5093d4(const CVector2D& a, const CVector2D& b, f32 t) {
    return Add_l5093d4(a, Scale_l5093d4(Sub_l5093d4(b, a), t));
}

void fn_71005093d4(UnkNavPoint_l5093d4* out, UnkNavMesh* mesh, const u16* cell, const u8* side, bool forward,
                   const CVector2D* widths, f32 t, UnkNavQuery_l5093d4 query) {
    fn_71002878dc(reinterpret_cast<UnkMsg_b152*>(out));
    if (*cell == 0xffff || *side == 0xff)
        return;
    auto* view = reinterpret_cast<UnkNavMeshView_l5093d4*>(mesh);
    UnkNavMeshCell* first = view->Cell(*cell);
    UnkNavMeshEdge* first_edge = fn_71002afbe4(first, side);
    UnkNavMeshSideSegment segment;
    segment.field_0x10 = -1.0f;
    segment.start.x = 0.0f;
    segment.start.y = 0.0f;
    segment.end.x = 0.0f;
    segment.end.y = 0.0f;
    fn_71002afea8(first, side, &segment);
    out->distance = 0.0f;
    out->pos = Lerp_l5093d4(segment.start, segment.end, t);
    out->cell = *cell;
    out->side = *side;
    auto* query_ = reinterpret_cast<UnkNavQuery_ovb0014*>(&query);
    if (!fn_71005091d4(query_, first, first_edge))
        return;
    const UnkNavEdge_l5093d4* edge = reinterpret_cast<const UnkNavEdge_l5093d4*>(first_edge);
    f32 distance = (forward ? 1.0f - t : t) * edge->length;
    if (!(distance < kMaxDistance)) {
        out->distance = kMaxDistance;
        CVector2D step = {edge->dir.x * kMaxDistance, edge->dir.y * kMaxDistance};
        out->pos = forward ? Add_l5093d4(out->pos, step) : Sub_l5093d4(out->pos, step);
        return;
    }
    out->distance = distance;
    const CVector2D& reached = forward ? segment.end : segment.start;
    out->pos.x = reached.x;
    out->pos.y = reached.y;

    UnkNavMeshSideSegment next_segment;
    const CVector2D* next_end = forward ? &next_segment.end : &next_segment.start;
    u16 next_cell = 0xffff;
    u8 next_side = 0xff;
    u16 at_cell = *cell;
    u8 at_side = *side;
    while (fn_710027c460(mesh, at_cell, at_side, forward, widths, 4, &next_cell, &next_side)) {
        UnkNavMeshCell* next = view->Cell(next_cell);
        UnkNavMeshEdge* next_edge = fn_71002afbe4(next, &next_side);
        if (!fn_71005091d4(query_, next, next_edge))
            return;
        next_segment.field_0x10 = -1.0f;
        next_segment.start.x = 0.0f;
        next_segment.start.y = 0.0f;
        next_segment.end.x = 0.0f;
        next_segment.end.y = 0.0f;
        fn_71002afea8(next, &next_side, &next_segment);
        edge = reinterpret_cast<const UnkNavEdge_l5093d4*>(next_edge);
        f32 total = out->distance + edge->length;
        if (!(total < kMaxDistance)) {
            f32 rest = kMaxDistance - out->distance;
            out->distance = kMaxDistance;
            CVector2D step = {rest * edge->dir.x, rest * edge->dir.y};
            out->pos = forward ? Add_l5093d4(next_segment.start, step) : Sub_l5093d4(next_segment.end, step);
            out->cell = next_cell;
            out->side = next_side;
            return;
        }
        out->pos.x = next_end->x;
        out->pos.y = next_end->y;
        at_cell = next_cell;
        at_side = next_side;
        next_cell = 0xffff;
        next_side = 0xff;
        out->distance = total;
        out->cell = at_cell;
        out->side = at_side;
    }
}
