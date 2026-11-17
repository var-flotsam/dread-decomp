// Unit 442 (tools/tu_map.py, no reliable name): where two 2D polylines meet. Starts from the projected
// position and the first edge of `a`; then, for every edge of `a`, finds an end point lying on an
// unflagged edge of `b` and keeps the one nearest the position (with its edge). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/malloc.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// A 2D point returned in memory: its copy constructor is user-written.
struct UnkPoint2D_l35f248 : CVector2D {
    UnkPoint2D_l35f248() {}
    UnkPoint2D_l35f248(const UnkPoint2D_l35f248& o) {
        x = o.x;
        y = o.y;
    }
};

struct UnkPolylineVertex_l35f248 {
    CVector2D pos;
    u32 field_0x8;
};

struct UnkPolyline_l35f248 {
    UnkPolylineVertex_l35f248* vertices;
    u32 count;
    u8 unk_0xc[0x20];
    u8 closed;  // 0x2c

    u32 EdgeCount() const { return count - (!closed & (count != 0)); }

    UnkSegment2D Edge(u64 i) const {
        const UnkPolylineVertex_l35f248* v = &vertices[i];
        return UnkSegment2D(v->pos, (i == count - 1 ? vertices : v + 1)->pos);
    }
};

static inline bool IsAbout(f32 a, f32 b, f32 eps) {
    return a - eps <= b && a + eps >= b;
}

static inline f32 DistSq(const CVector2D& a, const CVector2D& b) {
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;
    return dx * dx + dy * dy;
}

// The squared distance in the plane from the projected position to `pt`.
static inline f32 DistSqFrom(const CVector3D* pos, const CVector2D& pt) {
    UnkPoint2D_l35f248 p;
    fn_710007b430(&p, pos);
    return DistSq(p, pt);
}

UnkPoint2D_l35f248 fn_710035f248(const UnkPolyline_l35f248* a, const UnkPolyline_l35f248* b, const CVector3D* pos,
                                 UnkSegment2D* edge) {
    UnkPoint2D_l35f248 result;
    fn_710007b430(&result, pos);
    if (a->EdgeCount() != 0) {
        UnkSegment2D first = a->Edge(0);
        fn_7100125b44(edge, &first);
    }
    if (b == nullptr || b->EdgeCount() == 0)
        return result;

    u32 edges = a->EdgeCount();
    f32 best = -1.0f;
    for (u64 i = 0; i < edges; ++i) {
        UnkSegment2D seg = a->Edge(i);
        u32 others = b->EdgeCount();
        for (u64 j = 0; j < others; ++j) {
            if (b->vertices[j].field_0x8 != 0)
                continue;
            UnkSegment2D other = b->Edge(j);
            CVector2D hit;
            f32 d;
            if (fn_7100125c64(&other, &seg.start, false, 1.0f)) {
                d = DistSqFrom(pos, seg.start);
                hit = seg.start;
            } else if (fn_7100125c64(&other, &seg.end, false, 1.0f)) {
                d = DistSqFrom(pos, seg.end);
                hit = seg.end;
            } else {
                continue;
            }
            if (IsAbout(best, -1.0f, 0.1f) || d < best) {
                result.x = hit.x;
                result.y = hit.y;
                fn_7100125b44(edge, &seg);
                best = d;
            }
        }
    }
    return result;
}
