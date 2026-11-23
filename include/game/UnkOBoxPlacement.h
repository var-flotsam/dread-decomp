#pragma once

#include "base/spatial/COBox2D.h"

// The box copied member by member (as UnkPlacement2D in src/game/UnkCollisionContacts.cpp).
struct UnkOBoxPlacement : base::spatial::COBox2D {
    UnkOBoxPlacement(const UnkOBoxPlacement& o)
        : COBox2D{{o.center.x, o.center.y}, {o.axis_x.x, o.axis_x.y}, {o.axis_y.x, o.axis_y.y},
                  {o.size.x, o.size.y}, o.offset, o.angle} {}
};
