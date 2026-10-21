// A 2D segment's unit normal: the direction from start to end turned a quarter turn
// counter-clockwise, or zero for a degenerate segment.
#include <math.h>

#include "base/math/CVector.h"



// Returned through x8, so the original's result type is not trivially copyable.
struct UnkVec2Result {
    f32 x;
    f32 y;
    UnkVec2Result(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkVec2Result(const UnkVec2Result& o) : x(o.x), y(o.y) {}
};

struct UnkSegment2D {
    base::math::CVector2D start;
    base::math::CVector2D end;
};



UnkVec2Result fn_7100125e78(const UnkSegment2D* seg) {
    f32 dx = seg->end.x - seg->start.x;
    f32 dy = seg->end.y - seg->start.y;
    f32 len2 = dx * dx + dy * dy;
    if (len2 > 1.1920929e-7f) {
        f32 inv = 1.0f / sqrtf(len2);
        f32 nx = dx * inv;
        f32 ny = dy * inv;
        return UnkVec2Result(-ny, nx);
    }
    return UnkVec2Result(0.0f, 0.0f);
}
