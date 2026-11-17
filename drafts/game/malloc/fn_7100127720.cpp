// Unit 110 (2D geometry): two circles against each other. Coincident circles of one radius, and with
// `flag` one circle holding the other's centre, give one contact at a centre; touching circles give
// one contact and crossing circles two, each with the normals towards both centres.
// Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
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

// Appends a cleared point and returns its index (the count itself once it reaches ~0).
static inline u32 AddPoint(UnkContactBuffer<2>* out) {
    u32 i = out->count;
    if (i != 0xFFFFFFFF) {
        out->points[i] = UnkContactPoint();
        out->count = i + 1;
    }
    return i;
}

// The first point, added when the buffer is empty.
static inline UnkContactPoint& FirstPoint(UnkContactBuffer<2>* out) {
    if (out->count == 0) {
        out->points[0] = UnkContactPoint();
        out->count = 1;
    }
    return out->points[0];
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

// Makes the buffer hold one point at (x, y), with the normals along y.
static inline void SetCentrePoint(UnkContactBuffer<2>* out, f32 x, f32 y) {
    const f32 up_x = 0.0f;
    const f32 up_y = 1.0f;
    SetPoint(FirstPoint(out), x, y, up_x, up_y, -up_x, -up_y);
}

// The unit vector along (x, y), or (x, y) itself when it is zero.
static inline void Normalize(f32& x, f32& y) {
    f32 len = sqrtf(x * x + y * y);
    if (len != 0.0f) {
        x = x / len;
        y = y / len;
    }
}

bool fn_7100127720(const UnkCircle2D* a, const UnkCircle2D& b, UnkContactBuffer<2>* out, bool flag) {
    f32 ra = a->radius;
    f32 rb = b.radius;
    f32 dx = b.x - a->x;
    f32 dy = b.y - a->y;
    f32 dist = sqrtf(dx * dx + dy * dy);
    if (ra == rb && dist == 0.0f) {
        SetCentrePoint(out, a->x, a->y);
        return true;
    }
    f32 sum = ra + rb;
    if (dist > sum)
        return false;
    if (dist < fabsf(ra - rb)) {
        if (flag) {
            if (fn_710010f838(&b, reinterpret_cast<const UnkCapsule2D*>(a), 0.0f))
                SetCentrePoint(out, a->x, a->y);
            else if (fn_710010f838(a, reinterpret_cast<const UnkCapsule2D*>(&b), 0.0f))
                SetCentrePoint(out, b.x, b.y);
        }
        return out->count != 0;
    }

    // The chord through the crossing points: its distance from a's centre and its half length.
    f32 along = (ra * ra - rb * rb + dist * dist) / (dist + dist);
    f32 half = sqrtf(ra * ra - along * along);
    f32 nx = dx / dist;
    f32 ny = dy / dist;
    f32 mid_x = nx * along + a->x;
    f32 mid_y = ny * along + a->y;
    if (dist == sum) {
        f32 bx = -nx;
        f32 by = -ny;
        SetPoint(out->points[AddPoint(out)], mid_x, mid_y, nx, ny, bx, by);
        return true;
    }

    f32 off_x = nx * half;
    f32 off_y = ny * half;
    f32 p1x = off_y + mid_x;
    f32 p1y = mid_y - off_x;
    f32 a1x = p1x - a->x;
    f32 a1y = p1y - a->y;
    Normalize(a1x, a1y);
    f32 b1x = p1x - b.x;
    f32 b1y = p1y - b.y;
    Normalize(b1x, b1y);
    SetPoint(out->points[AddPoint(out)], p1x, p1y, a1x, a1y, b1x, b1y);

    f32 p2x = mid_x - off_y;
    f32 p2y = off_x + mid_y;
    f32 a2x = p2x - a->x;
    f32 a2y = p2y - a->y;
    Normalize(a2x, a2y);
    f32 b2x = p2x - b.x;
    f32 b2y = p2y - b.y;
    Normalize(b2x, b2y);
    SetPoint(out->points[AddPoint(out)], p2x, p2y, a2x, a2y, b2x, b2y);
    return true;
}
