// Unit 110 (2D geometry): a segment against a circle. The segment's line meets the circle where the
// quadratic along it has roots in [0, 1]; a touching line gives one contact, a crossing one up to two.
// With no contact and `flag` set, a segment start inside the circle gives a contact at its middle.
// Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector3D;

struct UnkCircle2D {
    f32 x, y;
    f32 radius;
};

// One contact point (0x2c bytes): the position, two vectors that belong to the first and the
// second shape of the pair (swapped when the pair is reported the other way round), and flags.
struct UnkContactPoint {
    CVector3D pos;
    CVector3D field_0xc;
    CVector3D field_0x18;
    u32 field_0x24;
    u32 flags;

    UnkContactPoint() = default;
    // Copied word by word (fn_7100d574c4 too).
    UnkContactPoint(const UnkContactPoint& o)
        : pos{o.pos.x, o.pos.y, o.pos.z}, field_0xc{o.field_0xc.x, o.field_0xc.y, o.field_0xc.z},
          field_0x18{o.field_0x18.x, o.field_0x18.y, o.field_0x18.z}, field_0x24(o.field_0x24), flags(o.flags) {}
};
static_assert(sizeof(UnkContactPoint) == 0x2c, "UnkContactPoint layout");

// The contact points a shape pair test writes: a count, then up to N points.
template <u32 N>
struct UnkContactBuffer {
    u32 count;
    UnkContactPoint points[N];
};

struct UnkCapsule2D;
bool fn_710010f838(const UnkCircle2D* circle, const UnkCapsule2D* capsule, f32 tolerance);

// The 2D tests' tolerance (0.001).
extern const f32 g_unk_7101647774;

static inline bool UnkNearEps(f32 a, f32 b) {
    f32 lo = a - g_unk_7101647774;
    f32 hi = a + g_unk_7101647774;
    return hi >= b && lo <= b;
}

// Appends a cleared point and returns its index (the count itself once it reaches ~0).
static inline u32 AddPoint(UnkContactBuffer<2>* out) {
    u32 i = out->count;
    if (i != 0xFFFFFFFF) {
        out->count = i + 1;
        out->points[i] = UnkContactPoint();
    }
    return i;
}

// A point at (x, y) with the normal of the first shape and that of the second.
static inline void SetPoint(UnkContactPoint& c, f32 x, f32 y, f32 ax, f32 ay, f32 bx, f32 by) {
    c.pos.x = x;
    c.pos.y = y;
    c.pos.z = 0.0f;
    c.field_0xc.x = ax;
    c.field_0xc.y = ay;
    c.field_0xc.z = 0.0f;
    c.field_0x18.x = bx;
    c.field_0x18.y = by;
    c.field_0x18.z = 0.0f;
    c.field_0x24 = 0;
    c.flags = 0x38;
}

// Appends a point at (x, y) with the normal of the first shape and that of the second.
static inline void AddContact(UnkContactBuffer<2>* out, f32 x, f32 y, f32 ax, f32 ay, f32 bx, f32 by) {
    SetPoint(out->points[AddPoint(out)], x, y, ax, ay, bx, by);
}

// Marks the point as set at (x, y).
static inline void MarkPoint(UnkContactPoint& c, f32 x, f32 y) {
    c.pos.x = x;
    c.pos.y = y;
    c.pos.z = 0.0f;
    c.flags |= 8;
}

// The unit vector along (x, y), or (x, y) itself when it is zero.
static inline void Normalize(f32& x, f32& y) {
    f32 len = sqrtf(x * x + y * y);
    if (len != 0.0f) {
        x = x / len;
        y = y / len;
    }
}

// The contact where the segment, at `t` along (dx, dy), crosses the circle: the segment's normal and
// the circle's outward normal there.
static inline void AddCrossing(const UnkSegment2D* seg, const UnkCircle2D* circle, UnkContactBuffer<2>* out,
                               f32 dx, f32 dy, f32 t) {
    f32 x = dx * t + seg->start.x;
    f32 y = dy * t + seg->start.y;
    UnkVec2Result normal = fn_7100125e78(seg);
    f32 out_x = x - circle->x;
    f32 out_y = y - circle->y;
    Normalize(normal.x, normal.y);
    Normalize(out_x, out_y);
    AddContact(out, x, y, normal.x, normal.y, out_x, out_y);
}

bool fn_71001271d0(const void* b, const UnkCircle2D* a, UnkContactBuffer<2>* out, bool flag) {
    const UnkSegment2D* seg = static_cast<const UnkSegment2D*>(b);
    if (UnkNearEps(seg->start.x, seg->end.x) && UnkNearEps(seg->start.y, seg->end.y))
        return false;
    f32 dx = seg->end.x - seg->start.x;
    f32 dy = seg->end.y - seg->start.y;
    f32 qa = dx * dx + dy * dy;
    if (fabsf(qa) <= 1.1920929e-11f)
        return false;

    // The points start + t * (dx, dy) on the circle: qa t^2 + qb t + qc = 0.
    f32 fx = seg->start.x - a->x;
    f32 fy = seg->start.y - a->y;
    f32 qb = 2.0f * (dx * fx + dy * fy);
    f32 qc = fx * fx + fy * fy - a->radius * a->radius;
    f32 disc = qb * qb + qa * -4.0f * qc;
    if (disc < 0.0f)
        return false;

    if (disc == 0.0f) {
        f32 t = qb / (qa * -2.0f);
        if (t >= 0.0f && t <= 1.0f) {
            f32 x = seg->start.x + dx * t;
            f32 y = seg->start.y + dy * t;
            f32 nx = a->x - x;
            f32 ny = a->y - y;
            Normalize(nx, ny);
            f32 bx = -nx;
            f32 by = -ny;
            AddContact(out, x, y, nx, ny, bx, by);
            return true;
        }
    } else {
        f32 root = sqrtf(disc);
        f32 neg_b = -qb;
        f32 t1 = (neg_b + root) / (qa + qa);
        if (t1 >= 0.0f && t1 <= 1.0f)
            AddCrossing(seg, a, out, dx, dy, t1);
        f32 t2 = (neg_b - root) / (qa + qa);
        if (t2 >= 0.0f && t2 <= 1.0f)
            AddCrossing(seg, a, out, dx, dy, t2);
    }

    if (out->count == 0 && flag) {
        if (fn_710010f838(a, static_cast<const UnkCapsule2D*>(b), 0.0f)) {
            if (out->count == 0)
                AddPoint(out);
            MarkPoint(out->points[0], (seg->start.x + seg->end.x) * 0.5f, (seg->start.y + seg->end.y) * 0.5f);
        }
    }
    return out->count != 0;
}
