#pragma once

// A 2D segment: start and end points, then two more words. fn_7100125b1c builds one from the two
// points; fn_7100125c64 (defined in src/game_lo/fn_7100125c64.cpp) tests whether a point lies on it.

#include "base/math/CVector.h"
#include "types.h"

struct UnkSegment2D {
    base::math::CVector2D start;
    base::math::CVector2D end;
    f32 field_0x10;
    u32 field_0x14;

    UnkSegment2D() = default;
    // Bound to the original's constructor, fn_7100125b1c (not defined in src/).
    UnkSegment2D(const base::math::CVector2D& s, const base::math::CVector2D& e) asm("fn_7100125b1c");
};
static_assert(sizeof(UnkSegment2D) == 0x18, "UnkSegment2D layout");

// The result type of fn_7100125e78, returned through x8 because the original's copy constructor is
// not trivial.
struct UnkVec2Result {
    f32 x;
    f32 y;
    UnkVec2Result(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkVec2Result(const UnkVec2Result& o) : x(o.x), y(o.y) {}
};

// The segment's unit normal: the direction turned a quarter turn counter-clockwise, or zero for a
// degenerate segment. Defined in src/game_lo/fn_7100125e78.cpp.
UnkVec2Result fn_7100125e78(const UnkSegment2D* seg);

// Whether the point lies on the segment within the tolerance; `extended` also accepts a point just
// past either end.
bool fn_7100125c64(const UnkSegment2D* seg, const base::math::CVector2D* point, bool extended, f32 tolerance);
