// From unit 108 of data/tu_map.csv (0x7100110c54-0x71001239b0, initialiser fn_7100122e30, no class
// hint), next to base::spatial::CPolygonCollection2D's: the nearest edge of a 2D polygon (its
// 0x48-byte layout is the one in src/base/spatial/CPolygonCollection2D.cpp) to a point, with an
// optional filter on the closest points and a callback for every edge measured.
// Holds fn_7100117f98 (the distance) and fn_7100117ba0 (the squared distance), and the bounding box
// update fn_7100119b88.
#include <math.h>

#include <functional>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;

struct UnkPolygonVertex {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkPolygon2D {
    UnkPolygonVertex* vertices;
    u32 count;
    u8 unk_0xc[0x20];
    u8 closed;
    bool box_dirty;  // 0x2d: the box below is stale
    u8 unk_0x2e[0x2];
    CVector2D box_min;  // 0x30
    CVector2D box_max;  // 0x38
    u8 unk_0x40[0x8];
};
static_assert(sizeof(UnkPolygon2D) == 0x48, "UnkPolygon2D layout");

// A filter on a candidate closest point, and a callback for each edge (its index and segment).
using UnkPointFilter = std::function<bool(const CVector2D&)>;
using UnkEdgeVisitor = std::function<void(u32, const UnkSegment2D&)>;

struct UnkSegment2D_71001262e0;  // a segment's two points (src/game/malloc/fn_71001262e0.cpp)
// The squared distance from `p` to the segment, with the closest point (and where along it).
f32 fn_71001262e0(const UnkSegment2D_71001262e0* seg, const CVector2D* p, CVector2D* closest, f32* t);
UnkSegment2D* fn_7100125b44(UnkSegment2D* self, const UnkSegment2D* other);

static inline u32 EdgeCount(const UnkPolygon2D* p) {
    return p->count - (!p->closed & (p->count != 0));
}

// The nearest edge to `point` among those whose closest point passes `filter`: its index (-1 for
// none), where along it the closest point lies (0 to 1), that point and the squared distance (each
// out parameter but the index optional). An edge as near as the one kept (within 0.001) replaces it
// when `flag` differs from whether it is the closing edge of a closed polygon with the first edge
// kept so far.
// NON_MATCHING(minor): register allocation and layout: the original also keeps the best point in
// two integer registers (and spills the t, closest and distance pointers instead), our vertex
// offset counts the edge's end instead of its start, and the loops' blocks are placed in another
// order.
void fn_7100117ba0(const UnkPolygon2D* polygon, const CVector2D* point, s32* edge, f32* t, CVector2D* closest,
                   f32* distance, bool flag, UnkPointFilter filter, UnkEdgeVisitor visit) {
    *edge = -1;
    *t = 0.0f;
    UnkSegment2D best;
    best.start.x = 0.0f;
    best.start.y = 0.0f;
    best.end.x = 0.0f;
    best.end.y = 0.0f;
    best.field_0x10 = -1.0f;
    f32 best_distance = 3.4028235e38f;
    CVector2D best_point = {0.0f, 0.0f};
    for (u64 i = 0; i < EdgeCount(polygon); ++i) {
        const UnkPolygonVertex* v = &polygon->vertices[i];
        UnkSegment2D seg(v->pos, (i == polygon->count - 1 ? polygon->vertices : v + 1)->pos);
        if (visit)
            visit(i, seg);
        CVector2D c = {0.0f, 0.0f};
        f32 d = fn_71001262e0(reinterpret_cast<const UnkSegment2D_71001262e0*>(&seg), point, &c, nullptr);
        if (d <= best_distance && (!filter || filter(c))) {
            if (d < best_distance ||
                (d + 0.001f >= best_distance && d - 0.001f <= best_distance &&
                 flag != (polygon->closed && i == polygon->count - 1 && *edge == 0))) {
                *edge = i;
                fn_7100125b44(&best, &seg);
                best_point = c;
                best_distance = d;
            }
        }
    }
    if (*edge != -1) {
        if (closest)
            *closest = best_point;
        f32 length = fn_7100125fbc(&best);
        f32 r;
        if (length > 1.1920929e-07f) {
            f32 dx = best_point.x - best.start.x;
            f32 dy = best_point.y - best.start.y;
            r = sqrtf(dx * dx + dy * dy) / length;
        } else {
            r = 0.0f;
        }
        *t = r;
    }
    if (distance)
        *distance = best_distance;
}

// fn_7100117ba0 with the distance itself.
void fn_7100117f98(const UnkPolygon2D* polygon, const CVector2D* point, s32* edge, f32* t, CVector2D* closest,
                   f32* distance, bool flag, UnkPointFilter filter, UnkEdgeVisitor visit) {
    fn_7100117ba0(polygon, point, edge, t, closest, distance, flag, filter, visit);
    if (distance)
        *distance = sqrtf(*distance);
}

// Recomputes the polygon's bounding box from its vertices (an empty polygon gets an inverted one) and
// marks it current.
void fn_7100119b88(UnkPolygon2D* self) {
    self->box_min.x = 3.4028235e+38f;
    self->box_min.y = 3.4028235e+38f;
    self->box_max.x = -3.4028235e+38f;
    self->box_max.y = -3.4028235e+38f;
    // Each corner is rebuilt from the vertex and stored whole on every pass; the stores may alias the
    // vertices (through references to the vectors), so the vertex is read again for the second.
    CVector2D& lo = self->box_min;
    CVector2D& hi = self->box_max;
    for (UnkPolygonVertex *v = self->vertices, *end = v + self->count; v != end; ++v) {
        const CVector2D& p = v->pos;
        f32 px = p.x;
        f32 py = p.y;
        f32 lx = lo.x < px ? lo.x : px;
        f32 ly = lo.y < py ? lo.y : py;
        lo.x = lx;
        lo.y = ly;
        px = p.x;
        py = p.y;
        f32 hx = hi.x > px ? hi.x : px;
        f32 hy = hi.y > py ? hi.y : py;
        hi.x = hx;
        hi.y = hy;
    }
    self->box_dirty = false;
}
