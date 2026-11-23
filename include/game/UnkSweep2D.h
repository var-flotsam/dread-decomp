#pragma once

#include "base/math/CVector.h"
#include "types.h"

struct UnkSweep2D {
    base::math::CVector2D start;
    base::math::CVector2D end;
    base::math::CVector2D delta;
};
