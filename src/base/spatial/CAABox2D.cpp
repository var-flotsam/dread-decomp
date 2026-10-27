// base::spatial::CAABox2D's unit (tools/tu_map.py unit 106: 0x710010c7a4-0x710010d604; its class
// registration is src/game_s1/fn_710010d540.cpp): an axis-aligned 2D box, its minimum corner then
// its maximum corner.
// Holds fn_710010d02c (a corner by index). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;

namespace base::spatial {

struct CAABox2D {
    CVector2D min;
    CVector2D max;
};

}  // namespace base::spatial

// Corner `index` of the box, clockwise from the minimum (y up): (min.x, min.y), (min.x, max.y),
// (max.x, max.y), (max.x, min.y); the origin for any other index.
UnkVec2Result fn_710010d02c(const base::spatial::CAABox2D* self, u32 index) {
    switch (index) {
    case 0:
        return UnkVec2Result(self->min.x, self->min.y);
    case 1:
        return UnkVec2Result(self->min.x, self->max.y);
    case 2:
        return UnkVec2Result(self->max.x, self->max.y);
    case 3:
        return UnkVec2Result(self->max.x, self->min.y);
    default:
        return UnkVec2Result(0.0f, 0.0f);
    }
}
