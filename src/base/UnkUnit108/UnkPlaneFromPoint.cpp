// From unit 108 of tools/tu_map.py: a plane from its normal and a point on it: the normal copied
// over a zeroed vector, then the distance -dot(point, normal). Names are placeholders.
#include "types.h"

typedef u32 __attribute__((may_alias)) UnkPlaneWord;
typedef u64 __attribute__((may_alias)) UnkPlaneDword;

struct UnkPlaneVec3 {
    f32 x, y, z;
};

struct UnkPlane {
    f32 x, y, z;
    f32 d;
};

f32 fn_7100110c54(UnkPlane* self, const UnkPlaneVec3* normal, const UnkPlaneVec3* point) {
    *reinterpret_cast<UnkPlaneDword*>(self) = 0;
    reinterpret_cast<UnkPlaneWord*>(self)[2] = 0;
    reinterpret_cast<UnkPlaneWord*>(self)[0] = reinterpret_cast<const UnkPlaneWord*>(normal)[0];
    reinterpret_cast<UnkPlaneWord*>(self)[1] = reinterpret_cast<const UnkPlaneWord*>(normal)[1];
    reinterpret_cast<UnkPlaneWord*>(self)[2] = reinterpret_cast<const UnkPlaneWord*>(normal)[2];
    f32 d = -(point->x * self->x + point->y * self->y + point->z * self->z);
    self->d = d;
    return d;
}
