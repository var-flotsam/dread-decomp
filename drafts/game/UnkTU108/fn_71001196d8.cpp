#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;

struct UnkPolyPoint {
    f32 x, y, z;
};
struct UnkPolygonBase {
    UnkPolyPoint* vertices;
    u32 count;
    __attribute__((always_inline)) UnkSegment2D Edge(u32 i) const {
        UnkPolyPoint* v = vertices;
        const UnkPolyPoint* cur = &v[i];
        const UnkPolyPoint* next = count - 1 == i ? v : &v[i + 1];
        return UnkSegment2D(*reinterpret_cast<const CVector2D*>(cur), *reinterpret_cast<const CVector2D*>(next));
    }
};
struct UnkPolygon2D : UnkPolygonBase {};

bool fn_7100119800(UnkPolygon2D* polygon, u32 vertex, const CVector2D* point, bool flag, f32 tolerance, void* a, void* b);

bool fn_71001196d8(UnkPolygon2D* polygon, u32 vertex, const CVector2D* point, bool flag, f32 tolerance, void* a,
                   void* b) {
    UnkSegment2D seg = polygon->Edge(vertex);
    f32 sx = seg.start.x;
    f32 sy = seg.start.y;
    f32 px = point->x - sx;
    f32 py = point->y - sy;
    f32 ey = seg.end.y - sy;
    f32 ex = seg.end.x - sx;
    f32 len2 = ex * ex + ey * ey;
    f32 t = (px * ex + py * ey) / len2;
    if (t < 0.0f)
        return fn_7100119800(polygon, vertex, &seg.start, flag, tolerance, a, b);
    if (t > 1.0f)
        return fn_7100119800(polygon, vertex, &seg.end, flag, tolerance, a, b);
    CVector2D on = {sx + ex * t, sy + ey * t};
    return fn_7100119800(polygon, vertex, &on, flag, tolerance, a, b);
}
