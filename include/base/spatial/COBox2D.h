#pragma once

// base::spatial::COBox2D (the name is the class registration's string, src/game_s1/fn_7100110b90.cpp;
// its unit is tools/tu_map.py unit 107, 0x710010d604-0x7100110c54, src/base/spatial/COBox2D.cpp): an
// oriented 2D box, its centre, its two unit axes (cos, sin and -sin, cos of the angle plus the
// offset), its size along them, then an angle offset and the angle. The oriented shapes of the 2D
// collision code hold one at +0xa8 (src/game/UnkCollisionContacts.cpp). Member names and the
// layout are placeholders inferred from the code.

#include "base/math/CVector.h"
#include "types.h"

namespace base::spatial {

struct COBox2D {
    base::math::CVector2D center;
    base::math::CVector2D axis_x;
    base::math::CVector2D axis_y;
    base::math::CVector2D size;  // along axis_x, along axis_y
    f32 offset;
    f32 angle;
};
static_assert(sizeof(COBox2D) == 0x28, "COBox2D placeholder layout");

}  // namespace base::spatial

// Sets the box's angle (wrapped into (-pi, pi]) and rebuilds its axes for the angle plus the
// offset. Defined in src/game_lo/fn_7100110664.cpp.
void fn_7100110664(base::spatial::COBox2D* self, f32 angle);
