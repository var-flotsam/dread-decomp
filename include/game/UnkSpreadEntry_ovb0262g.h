#pragma once

#include "base/math/CVector.h"
#include "types.h"

struct UnkSpreadEntry_ovb0262g {
    bool ok;  // 0x0
    base::math::CVector3D offset;  // 0x4
    f32 scale;  // 0x10
    f32 extra;  // 0x14
    UnkSpreadEntry_ovb0262g() : ok(false), offset{0.0f, 0.0f, 0.0f}, scale(0.0f), extra(0.0f) {}
};
