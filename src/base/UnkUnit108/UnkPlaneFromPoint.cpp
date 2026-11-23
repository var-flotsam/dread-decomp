// From unit 108 of tools/tu_map.py: a plane from its normal and a point on it: the normal copied
// over a zeroed vector, then the distance -dot(point, normal). Names are placeholders.
#include "base/UnkPlane.h"
#include "base/math/CVector.h"
#include "types.h"

typedef u32 UnkPlaneWord;
typedef u64 UnkPlaneDword;

using base::math::CVector3D;

f32 fn_7100110c54(UnkPlane* self, const CVector3D* normal, const CVector3D* point) {
    *reinterpret_cast<UnkPlaneDword*>(self) = 0;
    reinterpret_cast<UnkPlaneWord*>(self)[2] = 0;
    reinterpret_cast<UnkPlaneWord*>(self)[0] = reinterpret_cast<const UnkPlaneWord*>(normal)[0];
    reinterpret_cast<UnkPlaneWord*>(self)[1] = reinterpret_cast<const UnkPlaneWord*>(normal)[1];
    reinterpret_cast<UnkPlaneWord*>(self)[2] = reinterpret_cast<const UnkPlaneWord*>(normal)[2];
    f32 d = -(point->x * self->x + point->y * self->y + point->z * self->z);
    self->d = d;
    return d;
}
