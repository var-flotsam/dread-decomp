#pragma once

// base::math vector types. The names come from reflection (refl_GetType<base::math::CVector2D>
// fn_7100024504, refl_GetType<base::math::CVector3D> fn_71000246e4). The layouts are inferred
// from reflected default values, which store the components as consecutive floats.

#include "types.h"

namespace base::math {

struct CVector2D {
    f32 x;
    f32 y;
};
static_assert(sizeof(CVector2D) == 0x8, "CVector2D layout");

struct CVector3D {
    f32 x;
    f32 y;
    f32 z;
};
static_assert(sizeof(CVector3D) == 0xc, "CVector3D layout");

}  // namespace base::math

// Copies the x and y of a 3D vector into a 2D one (the projection).
void fn_710007b430(base::math::CVector2D* out, const base::math::CVector3D* v);
