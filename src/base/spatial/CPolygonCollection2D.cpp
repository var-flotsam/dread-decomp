// base::spatial::CPolygonCollection2D's unit (translation unit 109 of data/tu_map.csv; its static
// initialiser fn_71001258b0 registers the class): a pool of 0x48-byte polygons addressed by
// handles (bits 16 to 30 the polygon, the low 16 bits a vertex). fn_71001244ac tests a direction
// against the corner at a vertex.
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;

// A polygon's vertex: its position and one more word.
struct UnkPolygonVertex {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkPolygon2D {
    UnkPolygonVertex* vertices;
    u32 count;
    u8 unk_0xc[0x3c];

    // The edge from vertex i to the next one (the first after the last).
    __attribute__((always_inline)) UnkSegment2D Edge(u32 i) const {
        UnkPolygonVertex* v = vertices;
        const UnkPolygonVertex* next = count - 1 == i ? v : &v[i + 1];
        return UnkSegment2D(v[i].pos, next->pos);
    }
};
static_assert(sizeof(UnkPolygon2D) == 0x48, "UnkPolygon2D layout");

struct UnkPolygonCollection2D {
    UnkPolygon2D* polygons;
    u32 count;

    __attribute__((always_inline)) UnkPolygon2D* Find(u32 handle) {
        u32 index = (handle >> 16) & 0x7fff;
        if (index < count)
            return &polygons[index];
        return nullptr;
    }
};

// A probe: a position and, at 0xc, a direction.
struct UnkPolygonProbe {
    UnkVec2Result pos;
    f32 field_0x8;
    CVector2D dir;
};

// Where along a segment a point projects (0 at the start, 1 at the end).
f32 fn_71001261ac(const UnkSegment2D* seg, UnkVec2Result point);
// Whether the edge after a vertex continues past `dir`; fn_71001190a0 then gives that edge.
bool fn_7100118788(UnkPolygon2D* polygon, u32 vertex, const UnkVec2Result* dir);
UnkSegment2D fn_71001190a0(UnkPolygon2D* polygon, u32 vertex, const UnkVec2Result* dir, u32* a, u32* b);
UnkSegment2D* fn_7100125b44(UnkSegment2D* self, const UnkSegment2D* other);

// a - b, with a taken whole first.
static inline UnkVec2Result Diff(CVector2D a, const CVector2D& b) {
    return UnkVec2Result(a.x - b.x, a.y - b.y);
}

static inline f32 MaxAbs(f32 x, f32 y) {
    f32 ax = __builtin_fabsf(x);
    f32 ay = __builtin_fabsf(y);
    return ax < ay ? ay : ax;
}

// The edge that follows the vertex along `dir` (the next one when fn_7100118788 says so).
static inline UnkSegment2D FollowingEdge(UnkPolygon2D* polygon, u32 vertex, const UnkVec2Result* dir, u32* a,
                                         u32* b) {
    if (fn_7100118788(polygon, vertex, dir))
        return fn_71001190a0(polygon, vertex, dir, a, b);
    return polygon->Edge(vertex);
}

// True when the probe's position projects inside the edge at `handle`; at either end, whether its
// direction points into the corner there (within a tenth of each edge).
// NON_MATCHING(minor): the same calls, loads and arithmetic, but the branches' shared tail is merged
// differently (the original sinks only the last load of the adjacent edge into the join; ours more,
// keeping three more addresses in callee-saved registers), so the frame, stack slots and register
// allocation differ, and the probe's position is copied word by word instead of as a pair.
bool fn_71001244ac(UnkPolygonCollection2D* self, u32 handle, const UnkPolygonProbe* probe) {
    u32 a, b;
    u32 index = (handle >> 16) & 0x7fff;
    u32 vertex = handle & 0xffff;
    UnkSegment2D seg = self->polygons[index].Edge(vertex);
    f32 t = fn_71001261ac(&seg, probe->pos);
    if (t <= 0.99999f && t >= 1e-5f)
        return true;
    UnkVec2Result normal = fn_7100125e78(&seg);
    f32 nx = -probe->dir.x;
    f32 ny = -probe->dir.y;
    UnkSegment2D other;
    other.start.x = 0.0f;
    other.start.y = 0.0f;
    other.end.x = 0.0f;
    other.end.y = 0.0f;
    other.field_0x10 = -1.0f;
    UnkVec2Result edge(0.0f, 0.0f);
    f32 dx, dy;
    if (t > 0.99999f) {
        edge = Diff(seg.end, seg.start);
        UnkSegment2D next = FollowingEdge(self->Find(handle), vertex, &edge, &a, &b);
        fn_7100125b44(&other, &next);
        UnkVec2Result d = Diff(other.end, other.start);
        dx = d.x;
        dy = d.y;
    } else {
        edge = Diff(seg.start, seg.end);
        UnkSegment2D next = FollowingEdge(self->Find(handle), vertex, &edge, &a, &b);
        fn_7100125b44(&other, &next);
        UnkVec2Result d = Diff(other.start, other.end);
        dx = d.x;
        dy = d.y;
    }
    f32 dot = dx * normal.x + dy * normal.y;
    s32 side = (dot > 0.0f) - (dot < 0.0f);
    f32 sign = side ? (f32)side : 1.0f;
    if (!((dx * nx + dy * ny) * sign >= MaxAbs(dx, dy) * -0.1f))
        return false;
    return sign * (edge.x * nx + edge.y * ny) <= MaxAbs(edge.x, edge.y) * 0.1f;
}
