#pragma once

// base::color::CColor4F. The name and size (0x10) come from reflection (refl_GetType
// fn_7100068204); the layout is four floats. Its reflected constructor sets every component to 1
// and its reflected assignment calls fn_7100030728, which clamps each component to be
// non-negative. Its copy constructor is user-provided (a member-wise copy), so a colour passed or
// returned by value goes in memory (x8, or the address of a copy). Code here spells out the
// initial value instead of relying on the default constructor.

#include "base/math/CVector4D.h"
#include "types.h"

namespace base::color {

struct CColor4F {
    f32 r;
    f32 g;
    f32 b;
    f32 a;

    CColor4F() = default;
    CColor4F(f32 r_, f32 g_, f32 b_, f32 a_) : r(r_), g(g_), b(b_), a(a_) {}
    CColor4F(const CColor4F& other) : r(other.r), g(other.g), b(other.b), a(other.a) {}
};
static_assert(sizeof(CColor4F) == 0x10, "CColor4F layout");

}  // namespace base::color

// The colour's assignment (out of line): each component of `other` clamped to be non-negative.
void fn_7100030728(base::color::CColor4F* self, const base::color::CColor4F* other);

// The blend of two colours: `from` at t <= 0, `to` at t >= 1, linear in between; each component
// then clamped to be non-negative.
base::color::CColor4F fn_710003088c(f32 t, const base::color::CColor4F& from, const base::color::CColor4F& to);

// The colour's four components as a vector, by value (a copy).
base::math::UnkVector4D fn_7100030878(const base::color::CColor4F* color);
