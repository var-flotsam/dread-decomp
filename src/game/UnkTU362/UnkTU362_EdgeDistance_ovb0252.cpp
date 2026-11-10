// Unit 362 (data/tu_map.csv, no reliable name): a position on a navmesh cell side and its distance to
// the side's start, scaled by the edge's weight. Names and layouts are placeholders.
#include <math.h>

#include "game/UnkNavMeshCell.h"
#include "types.h"

struct UnkNavMeshPoint_b0252 {
    u16 cell;  // 0xffff for none
    u8 side;   // 0xff for none
    u8 pad_0x3;
    f32 x;  // 0x4
    f32 y;  // 0x8
};

struct UnkNavMeshEdgeWeight_b0252 {
    u8 pad_0x0[0x8];
    f32 weight;  // 0x8
};

f32 fn_7100287748(UnkNavMeshPoint_b0252* self, UnkNavMesh* mesh) {
    if (self->cell == 0xffff || self->side == 0xff)
        return -1.0f;
    UnkNavMeshEdge* edge = fn_71002afbe4(fn_710027a328(mesh, self->cell), &self->side);
    f32 weight = reinterpret_cast<UnkNavMeshEdgeWeight_b0252*>(edge)->weight;
    if (weight + 0.001f >= 0.0f && weight - 0.001f <= 0.0f)
        return 0.0f;
    UnkNavMeshSideSegment seg = {};
    seg.field_0x10 = -1.0f;
    fn_71002afea8(fn_710027a328(mesh, self->cell), &self->side, &seg);
    f32 dx = seg.start.x - self->x;
    f32 dy = seg.start.y - self->y;
    f32 dist = sqrtf(dx * dx + dy * dy);
    return dist / reinterpret_cast<UnkNavMeshEdgeWeight_b0252*>(edge)->weight;
}
