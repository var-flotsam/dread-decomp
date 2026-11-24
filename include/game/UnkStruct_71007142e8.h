#pragma once

#include "base/math/CVector.h"
#include "game/UnkXformNode.h"
#include "types.h"

// The motion fn_71007142e8 initialises and fn_71007143c8 resets: a transform (game/UnkXformNode.h), a
// mode word of motion-part bits, a vector and one more word. Readers of its translation take column 3 of
// the matrix (xform.m[i][3]). Names are placeholders.
struct UnkStruct_71007142e8 {
    UnkXform xform;  // 0x0
    u32 mode;  // 0x5c
    base::math::CVector3D vector;  // 0x60
    f32 field_0x6c;
};
static_assert(sizeof(UnkStruct_71007142e8) == 0x70, "layout");
