// Unit 442 (0x710035debc-0x710035fa0c, no class hint): the path an AI component follows
// (src/game/CAIComponent.cpp's UnkAIPathFollower): a 2D polygon, open or closed, with the vertex
// layout of src/base/spatial/UnkPolygon2D.cpp, and each edge's length at +0x48. Holds the side
// search fn_710035e8d8: the points of the path nearest to two points, the distance between them
// along the path and the direction to go. Names and layouts are placeholders inferred from the code.
#include <float.h>
#include <math.h>

#include <functional>
#include <new>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;

struct UnkAIPathVertex {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkAIPath {
    UnkAIPathVertex* vertices;
    u32 count;
    u8 unk_0xc[0x20];
    u8 closed;
    u8 unk_0x2d[0x1b];
    f32* lengths;  // one per edge

    u32 EdgeCount() const { return count - (!closed & (count != 0)); }
    // The edge after `i`, wrapping to the first.
    u32 NextEdge(u32 i) const { return i == EdgeCount() - 1 ? 0 : i + 1; }
    // The edge before `i`, wrapping to the last.
    u32 PrevEdge(u32 i) const { return (i == 0 ? EdgeCount() : i) - 1; }
    // An edge's length (-1 past the last edge).
    f32 EdgeLength(u32 i) const { return i < EdgeCount() ? lengths[i] : -1.0f; }
    // Builds edge `i` (from its vertex to the next one, wrapping to the first) in `seg`.
    void Edge(UnkSegment2D* seg, u32 i) const {
        new (seg) UnkSegment2D(vertices[i].pos, (count - 1 == i ? vertices : &vertices[i + 1])->pos);
    }
};

// The nearest edge of a polygon to a point (src/base/spatial/UnkPolygon2D.cpp, which defines the
// types).
struct UnkPolygon2D;
using UnkPointFilter = std::function<bool(const CVector2D&)>;
using UnkEdgeVisitor = std::function<void(u32, const UnkSegment2D&)>;
void fn_7100117f98(const UnkPolygon2D* polygon, const CVector2D* point, s32* edge, f32* t, CVector2D* closest,
                   f32* distance, bool flag, UnkPointFilter filter, UnkEdgeVisitor visit);
UnkSegment2D* fn_7100125b44(UnkSegment2D* self, const UnkSegment2D* other);
// The distance along an open path from (edge_from, t_from) to (edge_to, t_to).
f32 fn_710035e5f8(const UnkAIPath* self, u32 edge_from, u32 edge_to, f32 t_from, f32 t_to);

static inline bool IsNear(f32 a, f32 b) {
    return a - 0.001f <= b && a + 0.001f >= b;
}

static inline f32 Distance(f32 ax, f32 ay, f32 bx, f32 by) {
    f32 dx = ax - bx;
    f32 dy = ay - by;
    return sqrtf(dx * dx + dy * dy);
}

// The types src/game/CAIComponent.cpp declares this function with (its parameters are used as
// UnkAIPath, bool*, u32*, u32*, f32*, UnkSegment2D* and a std::function<bool(const CVector2D&)>).
struct UnkAIPathFollower;
struct UnkAIPathSearchOut;

// Finds the path's points nearest to `to` (through `filter`; no such point fails when there is a
// filter) and to `from`, their edges (`out_b`, `out_a`) and the distance between them along the path
// (`out_c`). On an open path the direction is toward the larger edge (or along `t` on one edge); on a
// closed one it is the shorter way round, the way against `flag` costing `limit` more. `out_0x7` gets
// whether the way is forward and `blocked` whether `dir_a` points against it; `point` gets the point
// nearest to `to` and `out` the edge of `from`.
// NON_MATCHING(minor): register allocation (the path in x23 instead of x24, the flag in w22, the
// direction in x21) and a frame 0x10 bytes smaller, so the locals and spills sit at other offsets;
// on the open path the tolerance test on t is an fccmp and csel instead of branches and the
// direction test is branch-free; `closest_to` is copied as one doubleword instead of two words.
bool fn_710035e8d8(UnkAIPathFollower* self, const CVector2D* from, const CVector2D* to, const CVector2D* dir_a,
                   const CVector2D* dir_b, bool flag, CVector2D* point, u32* out_0x7, f32 limit, bool* blocked,
                   f32* out_a, f32* out_b, u32* out_c, UnkAIPathSearchOut* out, std::function<void()> filter) {
    const UnkAIPath* path = reinterpret_cast<const UnkAIPath*>(self);
    const UnkPolygon2D* polygon = reinterpret_cast<const UnkPolygon2D*>(self);
    bool* forward = reinterpret_cast<bool*>(out_0x7);
    if (path->EdgeCount() == 0)
        return false;
    *blocked = false;
    *forward = true;
    s32 edge_to = -1;
    f32 t_to = -1.0f;
    CVector2D closest_to = *to;
    fn_7100117f98(polygon, to, &edge_to, &t_to, &closest_to, nullptr, false,
                  reinterpret_cast<const UnkPointFilter&>(filter), UnkEdgeVisitor());
    if (edge_to < 0 && filter)
        return false;
    *reinterpret_cast<s32*>(out_b) = edge_to;
    s32 edge_from = -1;
    f32 t_from = -1.0f;
    CVector2D closest_from = *from;
    fn_7100117f98(polygon, from, &edge_from, &t_from, &closest_from, nullptr, false, UnkPointFilter(),
                  UnkEdgeVisitor());
    if (edge_from < 0) {
        *reinterpret_cast<u32*>(out_a) = 0;
        return false;
    }
    *reinterpret_cast<s32*>(out_a) = edge_from;
    f32* distance = reinterpret_cast<f32*>(out_c);
    UnkSegment2D seg;
    if (!path->closed) {
        path->Edge(&seg, edge_from);
        f32 d;
        if (edge_to == edge_from) {
            f32 len = fn_7100125fbc(&seg);
            f32 r = len > FLT_EPSILON ? Distance(closest_from.x, closest_from.y, seg.start.x, seg.start.y) / len : 0.0f;
            if (!IsNear(t_to, r))
                flag = !(t_to < r);
            d = len * (t_to - t_from);
        } else {
            flag = edge_to > edge_from;
            d = fn_710035e5f8(path, edge_from, edge_to, t_from, t_to);
        }
        *distance = fabsf(d);
        *blocked = false;
        if (!IsNear(dir_a->x, 0.0f) || !IsNear(dir_a->y, 0.0f)) {
            f32 dot = (seg.end.x - seg.start.x) * dir_a->x + (seg.end.y - seg.start.y) * dir_a->y;
            if ((flag && dot < 0.0f) || (dot > 0.0f && !flag))
                *blocked = true;
        }
        *point = closest_to;
    } else {
        // The distances forward and back: on one edge the direct one (if not zero), the other way round
        // otherwise.
        f32 fwd = 0.0f;
        f32 back = 0.0f;
        if (edge_from == edge_to) {
            path->Edge(&seg, edge_from);
            f32 len = path->EdgeLength(edge_from);
            f32 r = len > FLT_EPSILON ? Distance(closest_from.x, closest_from.y, seg.start.x, seg.start.y) / len : 0.0f;
            if (t_to > r) {
                fwd = len * (t_to - r);
                back = FLT_MAX;
            } else {
                back = len * (r - t_to);
                fwd = FLT_MAX;
            }
        }
        if (fwd == 0.0f) {
            // From the end of the edge of `from` to the start of the edge of `to`.
            u32 i = path->NextEdge(edge_from);
            path->Edge(&seg, i);
            fwd = fwd + Distance(seg.start.x, seg.start.y, closest_from.x, closest_from.y);
            while (i != (u32)edge_to) {
                fwd = fwd + path->EdgeLength(i);
                i = path->NextEdge(i);
            }
            path->Edge(&seg, edge_to);
            fwd = fwd + Distance(closest_to.x, closest_to.y, seg.start.x, seg.start.y);
            fwd = flag ? fwd : fwd + limit;
        }
        if (back == 0.0f) {
            // From the start of the edge of `from` back to the end of the edge of `to`.
            u32 i = path->PrevEdge(edge_from);
            path->Edge(&seg, i);
            back = back + Distance(seg.end.x, seg.end.y, closest_from.x, closest_from.y);
            while (i != (u32)edge_to) {
                back = back + path->EdgeLength(i);
                i = path->PrevEdge(i);
            }
            path->Edge(&seg, edge_to);
            back = back + Distance(closest_to.x, closest_to.y, seg.end.x, seg.end.y);
            back = flag ? back + limit : back;
        }
        // The shorter way; ties go back.
        *distance = back <= fwd ? back : fwd;
        path->Edge(&seg, edge_from);
        if (!IsNear(dir_a->x, 0.0f) || !IsNear(dir_a->y, 0.0f)) {
            f32 dot = (seg.end.x - seg.start.x) * dir_a->x + (seg.end.y - seg.start.y) * dir_a->y;
            if ((!(back <= fwd) && dot < 0.0f) || (dot > 0.0f && back <= fwd))
                *blocked = true;
        }
        *point = closest_to;
        flag = !(back <= fwd);
    }
    *forward = flag;
    fn_7100125b44(reinterpret_cast<UnkSegment2D*>(out), &seg);
    return true;
}
