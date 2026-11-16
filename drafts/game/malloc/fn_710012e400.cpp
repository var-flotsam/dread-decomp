// Unit 110 (2D geometry): a moving circle against an oriented box: fn_710012deb0 in the box's frame
// (the circle's start and step turned into it, the box made aligned and centred), the contact taken
// back to the world's. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/malloc.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkPlacement2D;

// What the 2D tests fill on a hit (the layout of src/game/malloc/fn_710012be70.cpp's contact): the
// point, the normal of the first shape, the normal of the second, and which of them are set.
struct UnkContact2D {
    CVector3D point;
    CVector3D normal_a;
    CVector3D normal_b;
    f32 param;  // where along the moving shape's path (0x200)
    u32 flags;  // 8 point, 0x10 normal_b, 0x20 normal_a, 0x200 param

    void SetPoint(const CVector3D& p) {
        point = p;
        flags |= 8;
    }
    void SetNormalA(const CVector3D& n) {
        normal_a.x = n.x;
        normal_a.y = n.y;
        normal_a.z = n.z;
        flags |= 0x20;
    }
    void SetNormalB(const CVector3D& n) {
        normal_b.x = n.x;
        normal_b.y = n.y;
        normal_b.z = n.z;
        flags |= 0x10;
    }
    void SetNormalA(const UnkVec2Result& n) {
        normal_a.x = n.x;
        normal_a.y = n.y;
        normal_a.z = 0.0f;
        flags |= 0x20;
    }
    void SetNormalB(const UnkVec2Result& n) {
        normal_b.x = n.x;
        normal_b.y = n.y;
        normal_b.z = 0.0f;
        flags |= 0x10;
    }
};

// An axis-aligned box: its corners go (x0, y0), (x0, y1), (x1, y1), (x1, y0).
struct UnkBox2D {
    f32 x0;
    f32 y0;
    f32 x1;
    f32 y1;
    UnkVec2Result Corner(s32 index) const asm("fn_710010d02c");
};

struct UnkCircle2D {
    UnkVec2Result center;
    f32 radius;
    UnkCircle2D(const UnkVec2Result& c, f32 r) : center(c), radius(r) {}
};

// A moving point: where it starts, where it ends and the step between them.
struct UnkSweep2D {
    UnkVec2Result start;
    UnkVec2Result end;
    UnkVec2Result delta;
    UnkSweep2D() : start(0.0f, 0.0f), end(0.0f, 0.0f), delta(0.0f, 0.0f) {}
    UnkSweep2D(UnkVec2Result s, UnkVec2Result d)
        : start(s), end(d.x + s.x, d.y + s.y), delta(end.x - start.x, end.y - start.y) {}
    UnkSweep2D(const CVector2D& s, const CVector2D& e)
        : start(s.x, s.y), end(e.x, e.y), delta(end.x - start.x, end.y - start.y) {}
    UnkSweep2D& operator=(const UnkSweep2D& other) asm("fn_71001259f4");
};

static inline CVector2D UnkVec(f32 x, f32 y) {
    CVector2D v;
    v.x = x;
    v.y = y;
    return v;
}

// An oriented box: its centre, its two unit axes and its full size along them.
struct UnkOrientedBox2D {
    CVector2D center;
    CVector2D axis_x;
    CVector2D axis_y;
    CVector2D size;
    f32 field_0x20;  // the rotation is the sum of these two angles
    f32 field_0x24;
    // A vector in the box's frame: its dot products with the two axes.
    UnkVec2Result ToLocal(const CVector2D& v) const asm("fn_71001106cc");
    // A vector of the box's frame back in the world's: rotated by the box's angle.
    UnkVec2Result ToWorld(const CVector2D& v) const asm("fn_71001106f8");

    // A segment in the box's frame, as a moving point from its start to its end.
    UnkSweep2D ToLocal(const UnkSegment2D& seg) const {
        UnkSweep2D r;
        f32 mx = (seg.end.x + seg.start.x) * 0.5f;
        f32 my = (seg.end.y + seg.start.y) * 0.5f;
        CVector2D d = UnkVec(seg.end.x - seg.start.x, seg.end.y - seg.start.y);
        CVector2D rel = UnkVec(mx - center.x, my - center.y);
        UnkVec2Result mid = ToLocal(rel);
        UnkVec2Result dir = ToLocal(d);
        f32 hx = dir.x * 0.5f;
        f32 hy = dir.y * 0.5f;
        r = UnkSweep2D(UnkVec(mid.x - hx, mid.y - hy), UnkVec(hx + mid.x, hy + mid.y));
        return r;
    }
    // The same as a segment.
    UnkSegment2D ToLocalSegment(const UnkSegment2D& seg) const;
};

// A 3D vector's x and y (fn_710007b430), as a temporary.
struct UnkProjected : CVector2D {
    explicit UnkProjected(const CVector3D& v) { fn_710007b430(this, &v); }
};

static inline CVector3D UnkLiftAt(const UnkVec2Result& v, const CVector2D& origin) {
    return CVector3D{v.x + origin.x, v.y + origin.y, 0.0f};
}

// The step turned into the box's frame, kept apart from the call's result.
static inline UnkVec2Result LocalStep(const UnkOrientedBox2D* box, const CVector2D& step) {
    UnkVec2Result turned = box->ToLocal(step);
    return UnkVec2Result(turned);
}

// fn_710012deb0 in the box's frame: the circle's start and step turned into it, the box aligned and
// centred.
static inline bool HitInFrame(const UnkOrientedBox2D* box, const UnkCircle2D& circle, const CVector2D& move,
                              UnkContact2D* out) {
    f32 mx = (circle.center.x + circle.center.x + move.x) * 0.5f;
    f32 my = (circle.center.y + circle.center.y + move.y) * 0.5f;
    CVector2D rel = UnkVec(mx - box->center.x, my - box->center.y);
    UnkVec2Result mid = box->ToLocal(rel);
    UnkVec2Result localStep = LocalStep(box, move);
    f32 x1 = box->size.x * 0.5f;
    f32 y1 = box->size.y * 0.5f;
    f32 x0 = box->size.x * -0.5f;
    f32 y0 = box->size.y * -0.5f;
    UnkBox2D aabb = {x0, y0, x1, y1};
    f32 hx = localStep.x * 0.5f;
    f32 hy = localStep.y * 0.5f;
    UnkCircle2D local(UnkVec2Result(mid.x - hx, mid.y - hy), circle.radius);
    return fn_710012deb0(&local, &localStep, &aabb, out);
}

bool fn_710012e400(const UnkCircle2D& circle, void* x5, const UnkPlacement2D* a, UnkContactPoint* contact) {
    const CVector2D* move = static_cast<const CVector2D*>(x5);
    const UnkOrientedBox2D* box = reinterpret_cast<const UnkOrientedBox2D*>(a);
    UnkContact2D* out = reinterpret_cast<UnkContact2D*>(contact);

    bool hit = HitInFrame(box, circle, *move, out);
    if (out && hit) {
        out->SetPoint(UnkLiftAt(box->ToWorld(UnkProjected(out->point)), box->center));
        out->SetNormalA(box->ToWorld(UnkProjected(out->normal_a)));
        out->SetNormalB(box->ToWorld(UnkProjected(out->normal_b)));
    }
    return hit;
}
