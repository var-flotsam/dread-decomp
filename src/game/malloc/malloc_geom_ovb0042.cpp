// tools/tu_map.py unit 110: geometry tests. Whether a sphere (centre `p`, `radius`) touches an
// axis-aligned box, by the distance from the centre to its closest point in the box; and whether a
// direction lies between two segments' unit normals (fn_7100125e78 inlined). Names are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkAabb_b42 {
    CVector3D min;
    CVector3D max;
};

bool fn_7100126af0(const CVector3D* p, const UnkAabb_b42* box, f32 radius) {
    f32 cx = p->x;
    if (cx <= box->min.x)
        cx = box->min.x;
    else
        cx = cx >= box->max.x ? box->max.x : cx;
    f32 cy = p->y;
    if (cy <= box->min.y)
        cy = box->min.y;
    else
        cy = cy >= box->max.y ? box->max.y : cy;
    f32 cz = p->z;
    if (cz <= box->min.z)
        cz = box->min.z;
    else
        cz = cz >= box->max.z ? box->max.z : cz;
    f32 dx = p->x - cx;
    f32 dy = p->y - cy;
    f32 dz = p->z - cz;
    return dx * dx + dy * dy + dz * dz < radius * radius;
}

static inline UnkVec2Result Normal(const UnkSegment2D* seg) {
    f32 dx = seg->end.x - seg->start.x;
    f32 dy = seg->end.y - seg->start.y;
    f32 len2 = dx * dx + dy * dy;
    if (len2 > 1.1920929e-7f) {
        f32 inv = 1.0f / sqrtf(len2);
        f32 nx = dx * inv;
        f32 ny = dy * inv;
        return UnkVec2Result(-ny, nx);
    }
    return UnkVec2Result(0.0f, 0.0f);
}

bool fn_71001269ac(const UnkSegment2D* a, const UnkSegment2D* b, const CVector2D* p) {
    UnkVec2Result nb = Normal(b);
    UnkVec2Result na = Normal(a);
    f32 cross = nb.y * na.x - nb.x * na.y;
    f32 db = nb.x * p->x + nb.y * p->y;
    if (cross >= 0.0f) {
        if (!(db > 0.0f))
            return false;
    } else {
        if (db > 0.0f)
            return true;
    }
    return na.x * p->x + na.y * p->y > 0.0f;
}
