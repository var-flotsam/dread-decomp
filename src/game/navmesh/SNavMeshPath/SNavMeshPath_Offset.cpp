// In the navmesh::SNavMeshPath unit: moves the point at +0xc by an offset. Names and layouts are
// placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector3D;

struct UnkNavMeshPathPoint_2ac33c {
    u8 pad_0x0[0xc];
    CVector3D point;  // 0xc
};

void fn_71002ac33c(UnkNavMeshPathPoint_2ac33c* self, const CVector3D* offset) {
    self->point.x += offset->x;
    self->point.y += offset->y;
    self->point.z += offset->z;
}
