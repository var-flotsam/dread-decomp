// tools/tu_map.py unit 110: the distance from a point to a segment, the square root of
// fn_71001262e0's squared distance (src/game/malloc/fn_71001262e0.cpp). Names are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;

struct UnkSegment2D_71001262e0;  // a segment's two points (src/game/malloc/fn_71001262e0.cpp)

f32 fn_71001262e0(const UnkSegment2D_71001262e0* seg, const CVector2D* p, CVector2D* closest, f32* t);

f32 fn_71001262b4(const UnkSegment2D_71001262e0* seg, const CVector2D* p, CVector2D* closest, f32* t) {
    return sqrtf(fn_71001262e0(seg, p, closest, t));
}
