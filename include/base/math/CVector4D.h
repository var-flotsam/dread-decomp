#pragma once

// base::math::CVector4D. The name comes from reflection (refl_GetType<base::math::CVector4D>,
// fn_71000248c4); the layout is inferred from reflected default values (four consecutive
// floats, e.g. colours).

#include "types.h"

namespace base::math {

struct CVector4D {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
};
static_assert(sizeof(CVector4D) == 0x10, "CVector4D layout");

}  // namespace base::math
