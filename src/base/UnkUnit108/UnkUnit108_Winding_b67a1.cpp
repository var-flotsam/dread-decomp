// tools/tu_map.py unit 108 (no class hint): whether a polyline's points (12 bytes each, x and y used)
// wind one way: the trapezoid sum of (x1 - x0) / 2 * (y0 + y1) over its edges is not negative. The
// edge back to the first point counts only when the flag at +0x2c (closed) is set; an empty one
// counts as true (fn_7100118be8). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector3D;

struct UnkPolyline108_b67a1 {
    CVector3D* points;
    u32 count;  // 0x8
    u8 pad_0xc[0x20];
    bool closed;  // 0x2c
};

bool IsPolylineClockwiseXY(const UnkPolyline108_b67a1* self) {
    u32 edges = self->count;
    if (!self->closed && edges != 0)
        --edges;
    if (edges == 0)
        return true;
    f32 sum = 0.0f;
    for (u32 i = 0; i < edges; ++i) {
        const CVector3D& a = self->points[i];
        const CVector3D& b = self->points[i < edges - 1 ? i + 1 : 0];
        f32 ax = a.x, ay = a.y;
        f32 bx = b.x, by = b.y;
        sum += (bx - ax) * 0.5f * (ay + by);
    }
    return sum >= 0.0f;
}
