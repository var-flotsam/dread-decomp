// In the navmesh::SNavMeshPath unit: moves the point at +0xc by an offset. Names and layouts are
// placeholders.
#include "types.h"

struct UnkNavMeshPathVec3 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkNavMeshPathPoint {
    u8 pad_0x0[0xc];
    UnkNavMeshPathVec3 point;  // 0xc
};

void fn_71002ac33c(UnkNavMeshPathPoint* self, const UnkNavMeshPathVec3* offset) {
    self->point.x += offset->x;
    self->point.y += offset->y;
    self->point.z += offset->z;
}
