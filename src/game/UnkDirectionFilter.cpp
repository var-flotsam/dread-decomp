// A direction filter of unit 1111 (tools/tu_map.py; no class hint): whether a 2D segment faces a
// wanted direction, by the y of its unit normal or by the normal's dot product with the filter's
// direction (fn_71006f379c). Names and layout are placeholders inferred from the code.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkDirectionFilter {
    s32 mode;       // 2: compare with `direction`; otherwise with the up axis
    f32 threshold;  // -1 or below accepts every segment
    CVector3D direction;
};

// Whether `seg`'s unit normal is within the filter: its dot product with the filter's direction
// (projected to 2D and normalised) is at least the threshold in mode 2, else its y exceeds it.
bool fn_71006f379c(const UnkDirectionFilter* self, const UnkSegment2D* seg) {
    if (!(self->threshold > -1.0f))
        return true;
    s32 mode = self->mode;
    CVector3D direction;
    direction.x = self->direction.x;
    direction.y = self->direction.y;
    direction.z = self->direction.z;
    if (mode != 2) {
        UnkVec2Result normal = fn_7100125e78(seg);
        return normal.y > self->threshold;
    }
    CVector2D d;
    fn_710007b430(&d, &direction);
    f32 length = sqrtf(d.x * d.x + d.y * d.y);
    if (length != 0.0f) {
        d.x /= length;
        d.y /= length;
    }
    UnkVec2Result normal = fn_7100125e78(seg);
    return normal.x * d.x + normal.y * d.y >= self->threshold;
}
