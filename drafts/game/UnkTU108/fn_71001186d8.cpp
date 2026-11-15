// Unit 108: classifies a vector against the edge index of a closed polygon (the edge runs to the next
// point, wrapping): 2 when the projection is within tolerance of zero, else 1 when not positive, 0 when positive.
// Names and layouts are placeholders.
#include "base/math/UnkSegment2D.h"
#include "types.h"

struct UnkPolyPoint108 {
    f32 x, y, z;
};

struct UnkPolygon108 {
    UnkPolyPoint108* points;
    u32 count;  // 0x8
};

static inline const base::math::CVector2D& Pt(const UnkPolyPoint108* p) {
    return *reinterpret_cast<const base::math::CVector2D*>(p);
}

u32 fn_71001186d8(const UnkPolygon108* poly, u32 index, const base::math::CVector2D* v) {
    UnkPolyPoint108* points = poly->points;
    UnkPolyPoint108* current = &points[index];
    UnkPolyPoint108* next = (index == poly->count - 1) ? points : &points[index + 1];
    UnkSegment2D seg(Pt(current), Pt(next));
    f32 dx = seg.end.x - seg.start.x;
    f32 dy = seg.end.y - seg.start.y;
    f32 dot = dx * v->x + dy * v->y;
    u32 result = dot <= 0.0f;
    f32 low = dot + -0.00001f;
    f32 high = dot + 0.00001f;
    if (low <= 0.0f && high >= 0.0f)
        result = 2;
    return result;
}
