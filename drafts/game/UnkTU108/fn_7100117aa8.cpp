// Unit 108: whether the x range of any edge of a polygon comes within a tolerance of a value.
// A closed polygon has one edge per point (the last wraps to the first), an open one one fewer.
// Names and layouts are placeholders.
#include "base/math/UnkSegment2D.h"
#include "types.h"

struct UnkPolyPoint108 {
    f32 x, y, z;
};

struct UnkPolygon108 {
    UnkPolyPoint108* points;
    u32 count;  // 0x8
    u8 pad_0xc[0x20];
    u8 open;  // 0x2c

    u32 EdgeCount() const { return count - ((open == 0) & (count != 0)); }
};

static inline const base::math::CVector2D& Pt(const UnkPolyPoint108* p) {
    return *reinterpret_cast<const base::math::CVector2D*>(p);
}

bool fn_7100117aa8(UnkPolygon108* poly, const f32* value, f32 tolerance) {
    bool hit = false;
    for (u64 i = 0; !hit && i < poly->EdgeCount(); ++i) {
        UnkPolyPoint108* start = poly->points + i;
        UnkPolyPoint108* end = (i == poly->count - 1) ? poly->points : start + 1;
        UnkSegment2D seg(Pt(start), Pt(end));
        f32 lo = seg.start.x < seg.end.x ? seg.start.x : seg.end.x;
        f32 hi = seg.start.x > seg.end.x ? seg.start.x : seg.end.x;
        hit = lo <= *value + tolerance && hi >= *value - tolerance;
    }
    return hit;
}
