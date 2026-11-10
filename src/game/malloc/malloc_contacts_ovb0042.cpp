// tools/tu_map.py unit 110: shape pair tests built on other tests of the unit. The swapped tests
// run the test for the pair the other way round and then swap the two per-shape vectors of every
// contact point found, so they belong to the shapes in the caller's order; the swept ones do the
// same for a single point found with the step reversed and move it along the step by the time of
// impact; the capsule tests try the capsule's two end circles and its straight body in turn. Names
// and layouts are placeholders (the types are declared as src/game/UnkCollisionContacts.cpp does).
#include "base/math/UnkSegment2D.h"
#include "types.h"

struct UnkShape8Geometry;
struct UnkContactPoint;
struct UnkContact2D;
template <u32 N>
struct UnkContactBuffer;

// A circle: its centre and radius.
struct UnkCircle2D {
    f32 x, y;
    f32 radius;
};

// An axis-aligned box: its lower and upper corners.
struct UnkBox2D {
    f32 x0, y0;
    f32 x1, y1;
};

struct UnkVec3_b42 {
    f32 x, y, z;
};

// One contact point (0x2c bytes): the position, a vector for each shape of the pair, the time of
// impact of a swept test and flags.
struct UnkContactPoint_b42 {
    UnkVec3_b42 pos;
    UnkVec3_b42 vec_a;
    UnkVec3_b42 vec_b;
    f32 time;
    u32 flags;

    void SwapVectors() {
        f32 x = vec_a.x, y = vec_a.y, z = vec_a.z;
        vec_a.x = vec_b.x;
        vec_a.y = vec_b.y;
        vec_a.z = vec_b.z;
        vec_b.x = x;
        vec_b.y = y;
        vec_b.z = z;
    }
    // Moved along the step by the time of impact (the step has no z).
    void MoveAlong(const UnkVec2Result& step) {
        f32 sx = step.x, sy = step.y, sz = 0.0f;
        f32 t = time;
        pos.x = pos.x + sx * t;
        pos.y = pos.y + sy * t;
        pos.z = sz * t + pos.z;
        flags |= 8;
    }
    // The same, computed in another order.
    void Advance(const UnkVec2Result& step) {
        f32 t = time;
        f32 dx = t * step.x, dy = t * step.y, dz = 0.0f;
        f32 x = dx + pos.x, y = dy + pos.y, z = pos.z + dz;
        flags |= 8;
        pos.x = x;
        pos.y = y;
        pos.z = z;
    }
};
static_assert(sizeof(UnkContactPoint_b42) == 0x2c, "UnkContactPoint_b42 layout");

struct UnkContactList_b42 {
    u32 count;
    UnkContactPoint_b42 points[1];
};

bool fn_7100126bf0(const void* b, const UnkCircle2D* a, UnkContactBuffer<8>* out, bool flag);
bool fn_7100128510(const UnkBox2D* a, const void* b, UnkContactBuffer<2>* out, bool flag);
bool fn_7100129910(const void* b, const UnkShape8Geometry* a, UnkContactBuffer<4>* out, bool flag);
bool fn_71001271d0(const void* b, const UnkCircle2D* a, UnkContactBuffer<2>* out, bool flag);

bool fn_71001276b0(const UnkCircle2D* a, const void* b, UnkContactBuffer<8>* out, bool flag) {
    bool hit = fn_7100126bf0(b, a, out, flag);
    UnkContactList_b42* list = reinterpret_cast<UnkContactList_b42*>(out);
    for (u64 i = 0; i < list->count; i++)
        list->points[i].SwapVectors();
    return hit;
}

bool fn_710012b2c0(const void* b, const UnkBox2D* a, UnkContactBuffer<2>* out, bool flag) {
    bool hit = fn_7100128510(a, b, out, flag);
    UnkContactList_b42* list = reinterpret_cast<UnkContactList_b42*>(out);
    for (u64 i = 0; i < list->count; i++)
        list->points[i].SwapVectors();
    return hit;
}

bool fn_710012c760(const UnkShape8Geometry* a, const void* b, UnkContactBuffer<4>* out, bool flag) {
    bool hit = fn_7100129910(b, a, out, flag);
    UnkContactList_b42* list = reinterpret_cast<UnkContactList_b42*>(out);
    for (u64 i = 0; i < list->count; i++)
        list->points[i].SwapVectors();
    return hit;
}

bool fn_710012c7d0(const UnkCircle2D* a, const void* b, UnkContactBuffer<2>* out, bool flag) {
    bool hit = fn_71001271d0(b, a, out, flag);
    UnkContactList_b42* list = reinterpret_cast<UnkContactList_b42*>(out);
    for (u64 i = 0; i < list->count; i++)
        list->points[i].SwapVectors();
    return hit;
}

bool fn_710012f260(const UnkCircle2D* a, void* x5, const void* b, UnkContactPoint* out);
bool fn_710012ce70(const UnkBox2D* box, const UnkVec2Result& step, const UnkSegment2D* seg, UnkContact2D* out);

bool fn_710012f6a0(const void* b, void* x5, const UnkCircle2D* a, UnkContactPoint* out) {
    const UnkVec2Result& step = *static_cast<const UnkVec2Result*>(x5);
    UnkVec2Result back(-step.x, -step.y);
    bool hit = fn_710012f260(a, &back, b, out);
    if (out && hit) {
        UnkContactPoint_b42* p = reinterpret_cast<UnkContactPoint_b42*>(out);
        p->SwapVectors();
        p->MoveAlong(step);
    }
    return hit;
}

bool fn_710012d6b0(const void* b, void* x5, const UnkBox2D* a, UnkContactPoint* out) {
    const UnkVec2Result& step = *static_cast<const UnkVec2Result*>(x5);
    bool hit = fn_710012ce70(a, UnkVec2Result(-step.x, -step.y), static_cast<const UnkSegment2D*>(b),
                             reinterpret_cast<UnkContact2D*>(out));
    if (out && hit) {
        UnkContactPoint_b42* p = reinterpret_cast<UnkContactPoint_b42*>(out);
        p->Advance(step);
        p->SwapVectors();
    }
    return hit;
}

// A capsule standing on the y axis: its centre, the half length of its straight part and its radius.
struct UnkCapsule2D_b42 {
    f32 x, y;
    f32 half_height;
    f32 radius;

    UnkCircle2D Upper() const {
        UnkCircle2D c;
        f32 ox = 0.0f, oy = half_height;
        c.x = x + ox;
        c.y = y + oy;
        c.radius = radius;
        return c;
    }
    UnkCircle2D Lower() const {
        UnkCircle2D c;
        f32 cy = y - half_height;
        c.x = x;
        c.radius = radius;
        c.y = cy;
        return c;
    }
    UnkBox2D Body() const {
        UnkBox2D b;
        f32 hh = half_height, r = radius;
        f32 cx = x, cy = y;
        b.x0 = cx - r;
        b.y0 = cy - hh;
        b.x1 = r + cx;
        b.y1 = hh + cy;
        return b;
    }
};

bool fn_710012dd70(const UnkCircle2D* a, void* x5, const UnkCircle2D& b, UnkContactPoint* out);
bool fn_710012deb0(const UnkCircle2D* circle, const UnkVec2Result* delta, const UnkBox2D* box, UnkContact2D* out);
bool fn_710012e5c0(const UnkBox2D* box, const UnkVec2Result* delta, const UnkCircle2D* circle, UnkContact2D* out);

static inline const UnkContactPoint_b42* View(const UnkContactPoint* p) {
    return reinterpret_cast<const UnkContactPoint_b42*>(p);
}

static inline f32 Abs(f32 v) { return __builtin_fabsf(v); }

bool fn_710012f750(const UnkShape8Geometry* shape, const UnkVec2Result& step, const UnkSegment2D* seg,
                   UnkContactPoint* out) {
    const UnkCapsule2D_b42* capsule = reinterpret_cast<const UnkCapsule2D_b42*>(shape);
    {
        UnkBox2D body = capsule->Body();
        if (fn_710012ce70(&body, step, seg, reinterpret_cast<UnkContact2D*>(out))) {
            f32 v = Abs(View(out)->vec_a.x);
            f32 lo = v - 1e-5f;
            f32 hi = v + 1e-5f;
            if (lo <= 1.0f && hi >= 1.0f)
                return true;
        }
    }
    void* x5 = const_cast<UnkVec2Result*>(&step);
    {
        UnkCircle2D end = capsule->Upper();
        if (fn_710012f260(&end, x5, seg, out) && View(out)->vec_a.y >= 0.0f)
            return true;
    }
    {
        UnkCircle2D end = capsule->Lower();
        if (fn_710012f260(&end, x5, seg, out) && View(out)->vec_a.y <= 0.0f)
            return true;
    }
    return false;
}

bool fn_710012eb40(const UnkShape8Geometry* a, void* x5, const UnkCircle2D& b, UnkContactPoint* out) {
    const UnkCapsule2D_b42* capsule = reinterpret_cast<const UnkCapsule2D_b42*>(a);
    {
        UnkCircle2D end = capsule->Upper();
        if (fn_710012dd70(&end, x5, b, out) && View(out)->vec_a.y >= 0.0f)
            return true;
    }
    {
        UnkCircle2D end = capsule->Lower();
        if (fn_710012dd70(&end, x5, b, out) && View(out)->vec_a.y <= 0.0f)
            return true;
    }
    UnkBox2D body = capsule->Body();
    if (fn_710012e5c0(&body, static_cast<const UnkVec2Result*>(x5), &b, reinterpret_cast<UnkContact2D*>(out)) &&
        View(out)->vec_a.y == 0.0f)
        return true;
    return false;
}

bool fn_710012ec40(const UnkCircle2D& b, void* x5, const UnkShape8Geometry* a, UnkContactPoint* out) {
    const UnkCapsule2D_b42* capsule = reinterpret_cast<const UnkCapsule2D_b42*>(a);
    {
        UnkCircle2D end = capsule->Upper();
        if (fn_710012dd70(&b, x5, end, out) && View(out)->vec_b.y >= 0.0f)
            return true;
    }
    {
        UnkCircle2D end = capsule->Lower();
        if (fn_710012dd70(&b, x5, end, out) && View(out)->vec_b.y <= 0.0f)
            return true;
    }
    UnkBox2D body = capsule->Body();
    return fn_710012deb0(&b, static_cast<const UnkVec2Result*>(x5), &body, reinterpret_cast<UnkContact2D*>(out));
}
