// The contact stage of the 2D collision code (game::logic::collision): for a pair of shapes, each
// with its transform, the pair handler (picked by fn_7100d541c0 from the two shape types) computes
// the contact points, runs them past the two shapes' filter callbacks and reports the ones kept to
// a contact sink, one hit at a time, until the sink asks to stop (bit 1 of the result).
// Holds the reporting helpers fn_7100d53b2c (filter a point set, then report it), fn_7100d53d24
// (report a point set) and fn_7100d54020 (filter and report one point), and the pair handlers
// around them: oriented shapes (fn_7100d5337c to fn_7100d538fc), shapes against polygon
// collections (fn_7100d54a08 to fn_7100d54d78) and the other pairs (fn_7100d54f28 to fn_7100d5635c).
// Shape types: 1 segment (UnkSegment2D), 2 circle, 4 axis-aligned box, 8 a shape with a position and
// two more words, 0x10 polygon collection, 0x40 oriented shape. Names and layouts are placeholders.
// Definitions stay in address order: the dead-argument pass works through them in that order, which
// decides which unused first arguments callers still pass.
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/spatial/COBox2D.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector3D;

// The result of a contact query (same type as in src/game_s7/fn_7100d541c0.cpp): bit 0 keeps a
// point, bit 1 stops the query. Returned in memory: its copy is user-written.
struct UnkContact {
    u32 value;

    UnkContact() { value = 0; }
    UnkContact(const UnkContact& other) { value = other.value; }
};

inline const UnkContact& NoContact() {
    static UnkContact s_none;
    return s_none;
}

static inline UnkContact MakeContact(u32 value) {
    UnkContact c;
    c.value = value;
    return c;
}

// Both results' bits.
static inline UnkContact Merge(const UnkContact& a, const UnkContact& b) {
    return MakeContact(a.value | b.value);
}

// An empty argument (its register is left as it is).
struct UnkNoContext {};

struct UnkCollider;

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

struct UnkContactSpan {
    UnkContactPoint* data;
    u64 count;
};

// What the sink receives: a point moved by an offset, its flags with bit 3 set. Its own type, not
// a point inside a struct: the original reads the whole point before writing the hit. Passed by
// value; its copy is user-written, so it goes by reference on x86-64 too (as on AArch64).
struct UnkContactHit {
    s32 field_0x0;
    CVector3D pos;
    CVector3D field_0x10;
    CVector3D field_0x1c;
    u32 field_0x28;
    u32 flags;

    UnkContactHit() = default;
    UnkContactHit(const UnkContactHit& o)
        : field_0x0(o.field_0x0), pos(o.pos), field_0x10(o.field_0x10), field_0x1c(o.field_0x1c),
          field_0x28(o.field_0x28), flags(o.flags) {}
};
static_assert(sizeof(UnkContactHit) == 0x30, "UnkContactHit layout");

struct UnkContactSink {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkContact vf_0x10(UnkContactHit hit);
};

struct UnkShape;
// A shape's filter for the points of a point set and its filter for a single point; both are
// called with the shape (and its transform) first.
typedef UnkContact (*UnkPointFilter)(void* ctx, UnkShape* self, void* self_xf, UnkShape* other, void* other_xf,
                                     UnkContactPoint* point);
typedef UnkContact (*UnkSinglePointFilter)(void* ctx, UnkShape* self, void* self_xf, UnkShape* other, void* other_xf,
                                           const UnkContactPoint* point, void* extra, const CVector3D* offset);

struct UnkShape {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual u32 GetType();
    u8 unk_0x8[0x40];
    void* field_0x48;
    UnkPointFilter field_0x50;
    u8 unk_0x58[0x10];
    void* field_0x68;
    UnkSinglePointFilter field_0x70;
    u8 unk_0x78[0x30];
};

// UnkVector::PushBack over contact points, out of line.
void fn_7100d574c4(base::UnkVector<UnkContactPoint>* self, const UnkContactPoint* value);

UnkContact fn_7100d53d24(const UnkContactSpan* points, const CVector3D* offset, u16 flags, UnkContactSink* sink);

UnkContact fn_7100d53b2c(UnkCollider* self, const UnkContactSpan* points, UnkShape* a, void* ta, UnkShape* b, void* tb,
                         UnkContactSink* sink, const CVector3D* offset, u16 flags);
UnkContact fn_7100d54020(UnkNoContext, UnkContactPoint point, UnkShape* a, void* ta, UnkShape* b, void* tb,
                         UnkContactSink* sink, const CVector3D* offset, void* extra, u16 flags);

// The rotation angles of a transform (defined in src/game/UnkXformAngles.cpp).
UnkVec3f fn_7100076acc(const UnkXform* x);

// The 2D placement of an oriented shape (at +0xa8): its oriented box, base::spatial::COBox2D (the
// position, the rotation (cos, sin, -sin, cos) for the angle plus a stored offset, the size, that
// offset and the angle). src/engine/SystemStartup.cpp calls it UnkOrientedBox2D. The original copies
// it member by member (a plain copy of the box compiles to other code).
struct UnkPlacement2D : base::spatial::COBox2D {
    UnkPlacement2D(const UnkPlacement2D& o)
        : COBox2D{{o.center.x, o.center.y}, {o.axis_x.x, o.axis_x.y}, {o.axis_y.x, o.axis_y.y},
                  {o.size.x, o.size.y}, o.offset, o.angle} {}
};

// CVector3D is an aggregate; the original builds these through a constructor (the components are
// all computed before any is stored).
static inline CVector3D MakeVec3(f32 x, f32 y, f32 z) {
    CVector3D v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

static inline CVector3D Negated(const CVector3D& v) {
    CVector3D r;
    r.x = -v.x;
    r.y = -v.y;
    r.z = -v.z;
    return r;
}

static inline CVector3D Translation(const UnkXform* m) {
    return MakeVec3(m->m[0][3], m->m[1][3], m->m[2][3]);
}

static inline base::math::CVector2D Position2D(const UnkXform* m) {
    base::math::CVector2D v;
    v.x = m->m[0][3];
    v.y = m->m[1][3];
    return v;
}

// The contact points a shape pair test writes: a count, then up to N points.
template <u32 N>
struct UnkContactBuffer {
    u32 count;
    UnkContactPoint points[N];
};

struct UnkOrientedShape : UnkShape {
    UnkPlacement2D placement;
};

static_assert(sizeof(UnkShape) == 0xa8, "UnkShape layout");

// The shape-specific data that follows the base shape.
static inline const void* Geometry(const UnkShape* s) {
    return reinterpret_cast<const u8*>(s) + sizeof(UnkShape);
}

// A circle at the origin with the radius of the shape at +0xa8.
struct UnkCircle2D {
    f32 x, y;
    f32 radius;
};

static inline UnkCircle2D CircleAtOrigin(f32 radius) {
    UnkCircle2D c;
    c.x = 0.0f;
    c.y = 0.0f;
    c.radius = radius;
    return c;
}

// The geometry tests below that src/engine/SystemStartup.cpp defines keep its signatures: its
// UnkOrientedBox2D is UnkPlacement2D here, UnkContactList2D a UnkContactBuffer and UnkContact2D a
// UnkContactPoint.
struct UnkOrientedBox2D;
struct UnkContactList2D;
struct UnkContact2D;
bool fn_7100127bb0(const UnkOrientedBox2D* box, const UnkSegment2D* seg, UnkContactList2D* list, bool inside);
bool fn_7100127fc0(const UnkPlacement2D* a, const void* b, UnkContactBuffer<8>* out, bool flag);
bool fn_7100128da0(const UnkPlacement2D* a, const UnkCircle2D& b, UnkContactBuffer<8>* out, bool flag);
bool fn_7100128810(const UnkPlacement2D* a, const void* b, UnkContactBuffer<8>* out, bool flag);
bool fn_7100129360(const UnkPlacement2D* a, const void* b, UnkContactBuffer<8>* out, bool flag);

// The oriented shape `a` placed in b's frame (moved by the difference of the translations and turned
// by a's transform; fn_7100d53784 also turns it by b's), tested against b's geometry, and the points
// found reported through fn_7100d53b2c at b's translation, halfway between the two in z. The tests
// differ per shape pair. The first argument is passed through to fn_7100d53b2c, which ignores it.
UnkContact fn_7100d5337c(UnkCollider* self, UnkOrientedShape* a, void* ta, UnkShape* b, void* tb, UnkContactSink* sink,
                         u16 flags) {
    const UnkXform* xa = static_cast<const UnkXform*>(ta);
    const UnkXform* xb = static_cast<const UnkXform*>(tb);
    f32 dx = xa->m[0][3] - xb->m[0][3];
    f32 dy = xa->m[1][3] - xb->m[1][3];
    UnkPlacement2D local = a->placement;
    local.center.x = dx + local.center.x;
    local.center.y = dy + local.center.y;
    fn_7100110664(&local, fn_7100076acc(xa).z + local.angle);
    UnkContactBuffer<2> buf;
    buf.count = 0;
    fn_7100127bb0(reinterpret_cast<const UnkOrientedBox2D*>(&local), static_cast<const UnkSegment2D*>(Geometry(b)),
                  reinterpret_cast<UnkContactList2D*>(&buf), !(flags & 1));
    CVector3D offset = Translation(xb);
    offset.z = (xa->m[2][3] + xb->m[2][3]) * 0.5f;
    UnkContactSpan span = {buf.points, buf.count};
    return fn_7100d53b2c(self, &span, a, ta, b, tb, sink, &offset, flags);
}

UnkContact fn_7100d534d0(UnkCollider* self, UnkOrientedShape* a, void* ta, UnkShape* b, void* tb, UnkContactSink* sink,
                         u16 flags) {
    const UnkXform* xa = static_cast<const UnkXform*>(ta);
    const UnkXform* xb = static_cast<const UnkXform*>(tb);
    f32 dx = xa->m[0][3] - xb->m[0][3];
    f32 dy = xa->m[1][3] - xb->m[1][3];
    UnkPlacement2D local = a->placement;
    local.center.x = dx + local.center.x;
    local.center.y = dy + local.center.y;
    fn_7100110664(&local, fn_7100076acc(xa).z + local.angle);
    UnkContactBuffer<8> buf;
    buf.count = 0;
    fn_7100127fc0(&local, Geometry(b), &buf, !(flags & 1));
    CVector3D offset = Translation(xb);
    offset.z = (xa->m[2][3] + xb->m[2][3]) * 0.5f;
    UnkContactSpan span = {buf.points, buf.count};
    return fn_7100d53b2c(self, &span, a, ta, b, tb, sink, &offset, flags);
}

UnkContact fn_7100d53624(UnkCollider* self, UnkOrientedShape* a, void* ta, UnkShape* b, void* tb, UnkContactSink* sink,
                         u16 flags) {
    const UnkXform* xa = static_cast<const UnkXform*>(ta);
    const UnkXform* xb = static_cast<const UnkXform*>(tb);
    f32 dx = xa->m[0][3] - xb->m[0][3];
    f32 dy = xa->m[1][3] - xb->m[1][3];
    UnkPlacement2D local = a->placement;
    local.center.x = dx + local.center.x;
    local.center.y = dy + local.center.y;
    fn_7100110664(&local, fn_7100076acc(xa).z + local.angle);
    UnkContactBuffer<8> buf;
    buf.count = 0;
    fn_7100128da0(&local, CircleAtOrigin(*static_cast<const f32*>(Geometry(b))), &buf, !(flags & 1));
    CVector3D offset = Translation(xb);
    offset.z = (xa->m[2][3] + xb->m[2][3]) * 0.5f;
    UnkContactSpan span = {buf.points, buf.count};
    return fn_7100d53b2c(self, &span, a, ta, b, tb, sink, &offset, flags);
}

UnkContact fn_7100d53784(UnkCollider* self, UnkOrientedShape* a, void* ta, UnkShape* b, void* tb, UnkContactSink* sink,
                         u16 flags) {
    const UnkXform* xa = static_cast<const UnkXform*>(ta);
    const UnkXform* xb = static_cast<const UnkXform*>(tb);
    f32 dx = xa->m[0][3] - xb->m[0][3];
    f32 dy = xa->m[1][3] - xb->m[1][3];
    UnkPlacement2D local = a->placement;
    local.center.x = dx + local.center.x;
    local.center.y = dy + local.center.y;
    fn_7100110664(&local, fn_7100076acc(xa).z + local.angle);
    const void* geom = Geometry(b);
    fn_7100110664(&local, fn_7100076acc(xb).z + local.angle);
    UnkContactBuffer<8> buf;
    buf.count = 0;
    fn_7100128810(&local, geom, &buf, !(flags & 1));
    CVector3D offset = Translation(xb);
    offset.z = (xa->m[2][3] + xb->m[2][3]) * 0.5f;
    UnkContactSpan span = {buf.points, buf.count};
    return fn_7100d53b2c(self, &span, a, ta, b, tb, sink, &offset, flags);
}

UnkContact fn_7100d538fc(UnkCollider* self, UnkOrientedShape* a, void* ta, UnkShape* b, void* tb, UnkContactSink* sink,
                         u16 flags) {
    const UnkXform* xa = static_cast<const UnkXform*>(ta);
    const UnkXform* xb = static_cast<const UnkXform*>(tb);
    f32 dx = xa->m[0][3] - xb->m[0][3];
    f32 dy = xa->m[1][3] - xb->m[1][3];
    UnkPlacement2D local = a->placement;
    local.center.x = dx + local.center.x;
    local.center.y = dy + local.center.y;
    fn_7100110664(&local, fn_7100076acc(xa).z + local.angle);
    UnkContactBuffer<8> buf;
    buf.count = 0;
    fn_7100129360(&local, Geometry(b), &buf, !(flags & 1));
    CVector3D offset = Translation(xb);
    offset.z = (xa->m[2][3] + xb->m[2][3]) * 0.5f;
    UnkContactSpan span = {buf.points, buf.count};
    return fn_7100d53b2c(self, &span, a, ta, b, tb, sink, &offset, flags);
}

UnkContact fn_7100d53b2c(UnkCollider*, const UnkContactSpan* points, UnkShape* a, void* ta, UnkShape* b, void* tb,
                         UnkContactSink* sink, const CVector3D* offset, u16 flags) {
    base::UnkVector<UnkContactPoint> kept(0xf);
    u32 n = (u32)points->count;
    kept.Reserve(n);
    if (!(flags & 4)) {
        for (u32 i = 0; i < n; ++i) {
            UnkContactPoint* p = &points->data[i];
            UnkContact keep = MakeContact(1);
            if (a && a->field_0x50)
                keep = a->field_0x50(a->field_0x48, a, ta, b, tb, p);
            if (b && b->field_0x50)
                keep = b->field_0x50(b->field_0x48, b, tb, a, ta, p);
            if (keep.value & 1)
                fn_7100d574c4(&kept, p);
        }
    }
    UnkContactSpan span = {kept.data, kept.count};
    UnkContact result = fn_7100d53d24(&span, offset, flags, sink);
    kept.Free();
    return result;
}

// The two shape vectors are copied as raw words (the original selects them in integer registers).
static inline u32 Bits(const f32* f) {
    u32 v;
    __builtin_memcpy(&v, f, 4);
    return v;
}
static inline void SetBits(f32* f, u32 v) {
    __builtin_memcpy(f, &v, 4);
}
static inline void CopySwapped(CVector3D* out_a, CVector3D* out_b, const CVector3D* a, const CVector3D* b,
                               bool swapped) {
    u32 ax = Bits(&a->x), ay = Bits(&a->y), az = Bits(&a->z);
    u32 bx = Bits(&b->x), by = Bits(&b->y), bz = Bits(&b->z);
    SetBits(&out_a->x, swapped ? bx : ax);
    SetBits(&out_a->y, swapped ? by : ay);
    SetBits(&out_a->z, swapped ? bz : az);
    SetBits(&out_b->x, swapped ? ax : bx);
    SetBits(&out_b->y, swapped ? ay : by);
    SetBits(&out_b->z, swapped ? az : bz);
}

// The hit for a point: moved by the offset, the two shape vectors swapped when the pair is
// reported the other way round.
static inline void MakeHit(UnkContactHit* hit, const UnkContactPoint* p, const CVector3D* offset, bool swapped) {
    hit->field_0x0 = -1;
    CVector3D pos = p->pos;
    hit->pos.x = pos.x + offset->x;
    hit->pos.y = pos.y + offset->y;
    hit->pos.z = pos.z + offset->z;
    CopySwapped(&hit->field_0x10, &hit->field_0x1c, &p->field_0xc, &p->field_0x18, swapped);
    hit->field_0x28 = p->field_0x24;
    hit->flags = p->flags | 8;
}

static inline UnkContactHit HitFor(const UnkContactPoint* p, const CVector3D* offset) {
    UnkContactHit hit;
    MakeHit(&hit, p, offset, false);
    return hit;
}

// One point reported to the sink as it is (3 without a sink).
static inline UnkContact ReportPoint(UnkContactSink* sink, const UnkContactPoint* p, const CVector3D* offset) {
    return sink ? sink->vf_0x10(HitFor(p, offset)) : MakeContact(3);
}

// NON_MATCHING(minor): the two loop bodies address the point from each other's base offset (0x14
// and 0x28 swapped), so the same loads pair and schedule differently, and the offset is added to the
// position with the fadd operands the other way round.
UnkContact fn_7100d53d24(const UnkContactSpan* points, const CVector3D* offset, u16 flags, UnkContactSink* sink) {
    UnkContact result = NoContact();
    if (!sink) {
        if (points->count != 0)
            result.value = 3;
        return result;
    }
    for (u64 i = 0; !(result.value & 2) && i < points->count; ++i) {
        UnkContactHit hit;
        MakeHit(&hit, &points->data[i], offset, flags & 2);
        result.value |= sink->vf_0x10(hit).value;
    }
    return result;
}

// NON_MATCHING(minor): the filters' result temporary sits at sp+0x38 instead of sharing the hit's
// slot at sp+8; the hit's loads, selects (csel on the inverse condition) and stores are scheduled in
// another order, and the offset is added to the position with the fadd operands the other way round.
UnkContact fn_7100d54020(UnkNoContext, UnkContactPoint point, UnkShape* a, void* ta, UnkShape* b, void* tb,
                         UnkContactSink* sink, const CVector3D* offset, void* extra, u16 flags) {
    UnkContact result = MakeContact(1);
    if (!(flags & 4)) {
        if (a && a->field_0x70)
            result = a->field_0x70(a->field_0x68, a, ta, b, tb, &point, extra, offset);
        if (b && (result.value & 1) && b->field_0x70)
            result = b->field_0x70(b->field_0x68, b, tb, a, ta, &point, extra, offset);
        if (!(result.value & 1))
            return result;
    }
    UnkContactHit hit;
    MakeHit(&hit, &point, offset, flags & 2);
    result = sink ? sink->vf_0x10(hit) : MakeContact(3);
    return result;
}

// The query a polygon collection shape (type 0x10) runs for a shape placed in its frame: the
// translation to report the points at, the collection's data and the sink. Same type as in
// src/game_hi/fn_7100d56604.cpp (vtable at 0x7101aa8870).
struct UnkLocalQuery {
    virtual void vf_0x0();
    CVector3D offset;
    u16 field_0x14;
    const void* field_0x18;
    void* field_0x20;
    UnkLocalQuery(const CVector3D& o, const void* p, void* a)
        : offset(o), field_0x14(0), field_0x18(p), field_0x20(a) {}
};

// The 2D geometry of a type-8 shape: a position and two more words.
struct UnkShape8Geometry {
    f32 x, y;
    u32 field_0x8;
    u32 field_0xc;
    UnkShape8Geometry(const UnkShape8Geometry& o) : x(o.x), y(o.y), field_0x8(o.field_0x8), field_0xc(o.field_0xc) {}

    // The geometry moved by (dx, dy).
    static UnkShape8Geometry Placed(const UnkShape* s, f32 dx, f32 dy) {
        UnkShape8Geometry local = *static_cast<const UnkShape8Geometry*>(Geometry(s));
        local.x = dx + local.x;
        local.y = dy + local.y;
        return local;
    }
};

// An axis-aligned box (type 4).
struct UnkBox2D {
    f32 min_x, min_y;
    f32 max_x, max_y;

    static UnkBox2D Make(f32 min_x, f32 min_y, f32 max_x, f32 max_y) {
        UnkBox2D b;
        b.min_x = min_x;
        b.min_y = min_y;
        b.max_x = max_x;
        b.max_y = max_y;
        return b;
    }

    static UnkBox2D Placed(const UnkShape* s, f32 dx, f32 dy) {
        UnkBox2D local = *static_cast<const UnkBox2D*>(Geometry(s));
        local.min_x = dx + local.min_x;
        local.min_y = dy + local.min_y;
        local.max_x = dx + local.max_x;
        local.max_y = dy + local.max_y;
        return local;
    }
};

// A circle shape (type 2) keeps only its radius; placed, it is centred on (dx, dy).
struct UnkPlacedCircle : UnkCircle2D {
    static UnkCircle2D Placed(const UnkShape* s, f32 dx, f32 dy) {
        UnkCircle2D local = CircleAtOrigin(*static_cast<const f32*>(Geometry(s)));
        local.x = dx + local.x;
        local.y = dy + local.y;
        return local;
    }
};

typedef UnkContact (*UnkCollectionPairFn)(UnkCollider* self, UnkShape* a, void* ta, UnkShape* b, void* tb, void* sink,
                                          u32 x6);
UnkContact fn_7100d5244c(UnkCollider* self, UnkShape* a, void* ta, UnkShape* b, void* tb, void* sink, u32 x6);
UnkContact fn_7100d52bd0(UnkCollider* self, UnkShape* a, void* ta, UnkShape* b, void* tb, void* sink, u32 x6);
UnkContact fn_7100d5283c(UnkCollider* self, UnkShape* a, void* ta, UnkShape* b, void* tb, void* sink, u32 x6);
UnkContact fn_7100d656a0(UnkShape* self, const UnkShape8Geometry* shape, const CVector3D* dir, UnkLocalQuery* query);
UnkContact fn_7100d65200(UnkShape* self, const UnkBox2D* shape, const CVector3D* dir, UnkLocalQuery* query);
UnkContact fn_7100d64d3c(UnkShape* self, const UnkCircle2D* shape, const CVector3D* dir, UnkLocalQuery* query);

// The shape `a` (with its transform) against the polygon collection `b`: the shape is moved into
// b's frame and queried along the direction `x5` (reversed when the pair is swapped). With `flag`,
// the contacts of the pair handler `Pair` (b first) come first.
template <class G, class Placer, UnkCollectionPairFn Pair,
          UnkContact (*Query)(UnkShape*, const G*, const CVector3D*, UnkLocalQuery*)>
static inline UnkContact QueryCollection(UnkCollider* self, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5,
                                         void* x6, bool swapped, bool flag) {
    const UnkXform* xa = static_cast<const UnkXform*>(ta);
    const UnkXform* xb = static_cast<const UnkXform*>(tb);
    const base::math::CVector2D* d = static_cast<const base::math::CVector2D*>(x5);
    UnkContact result = NoContact();
    if (flag)
        result = Pair(self, b, tb, a, ta, x6, 6);
    CVector3D pa = Translation(xa);
    CVector3D pb = Translation(xb);
    G local = Placer::Placed(a, pa.x - pb.x, pa.y - pb.y);
    f32 dir_y = d->y;
    f32 dir_x = d->x;
    CVector3D dir = MakeVec3(dir_x, dir_y, 0.0f);
    if (swapped)
        dir = Negated(dir);
    UnkLocalQuery query(MakeVec3(xb->m[0][3], xb->m[1][3], (xa->m[2][3] + xb->m[2][3]) * 0.5f), Geometry(b), x6);
    result = Merge(result, Query(b, &local, &dir, &query));
    return result;
}

UnkContact fn_7100d54a08(UnkCollider* self, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6,
                         bool swapped, bool flag) {
    return QueryCollection<UnkShape8Geometry, UnkShape8Geometry, fn_7100d5244c, fn_7100d656a0>(self, a, ta, b, tb, x5,
                                                                                               x6, swapped, flag);
}

UnkContact fn_7100d54bc0(UnkCollider* self, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6,
                         bool swapped, bool flag) {
    return QueryCollection<UnkBox2D, UnkBox2D, fn_7100d52bd0, fn_7100d65200>(self, a, ta, b, tb, x5, x6, swapped, flag);
}

// NON_MATCHING(minor): the placed circle's radius is stored after its centre instead of before.
UnkContact fn_7100d54d78(UnkCollider* self, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6,
                         bool swapped, bool flag) {
    const UnkXform* xa = static_cast<const UnkXform*>(ta);
    const UnkXform* xb = static_cast<const UnkXform*>(tb);
    const base::math::CVector2D* d = static_cast<const base::math::CVector2D*>(x5);
    UnkContact result = NoContact();
    if (flag)
        result = fn_7100d5283c(self, b, tb, a, ta, x6, 6);
    CVector3D pa = Translation(xa);
    CVector3D pb = Translation(xb);
    UnkCircle2D local = UnkPlacedCircle::Placed(a, pa.x - pb.x, pa.y - pb.y);
    f32 dir_y = d->y;
    f32 dir_x = d->x;
    CVector3D dir = MakeVec3(dir_x, dir_y, 0.0f);
    if (swapped)
        dir = Negated(dir);
    UnkLocalQuery query(MakeVec3(xb->m[0][3], xb->m[1][3], (xa->m[2][3] + xb->m[2][3]) * 0.5f), Geometry(b), x6);
    result = Merge(result, fn_7100d64d3c(b, &local, &dir, &query));
    return result;
}

bool fn_71001271d0(const void* b, const UnkCircle2D* a, UnkContactBuffer<2>* out, bool flag);
bool fn_7100128510(const UnkBox2D* a, const void* b, UnkContactBuffer<2>* out, bool flag);
bool fn_710012f260(const UnkCircle2D* a, void* x5, const void* b, UnkContactPoint* out);
bool fn_710012f6a0(const void* b, void* x5, const UnkCircle2D* a, UnkContactPoint* out);
bool fn_710012ce70(const UnkBox2D* box, const UnkVec2Result& step, const UnkSegment2D* seg, UnkContact2D* out);
bool fn_710012d6b0(const void* b, void* x5, const UnkBox2D* a, UnkContactPoint* out);

// The shape `a` placed in b's frame against b's contact point set (`SetTest`), reported through
// fn_7100d53b2c.
template <class G, class Placer, class SetTest>
static inline UnkContact TestSet(UnkCollider* self, UnkShape* a, const UnkXform* ta, UnkShape* b, const UnkXform* tb,
                                 UnkContactSink* sink, u16 flags, SetTest test) {
    CVector3D pa = Translation(ta);
    CVector3D pb = Translation(tb);
    G local = Placer::Placed(a, pa.x - pb.x, pa.y - pb.y);
    UnkContactBuffer<2> buf;
    buf.count = 0;
    test(&local, Geometry(b), &buf);
    CVector3D offset = Translation(tb);
    offset.z = (ta->m[2][3] + offset.z) * 0.5f;
    UnkContactSpan span = {buf.points, buf.count};
    return fn_7100d53b2c(self, &span, a, (void*)ta, b, (void*)tb, sink, &offset, flags);
}

// The shape `a` against a type-1 shape `b`: with `flag`, b's point set against `a` first (FlagSet);
// then the single contact point of the pair along `x5` (Test, or TestReversed with the arguments the
// other way round when the pair is swapped).
template <class G, class Placer, class FlagSet, class Test, class TestReversed>
static inline UnkContact TestSingle(UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6, bool swapped,
                                    bool flag, FlagSet flag_set, Test test, TestReversed test_reversed) {
    const UnkXform* xa = static_cast<const UnkXform*>(ta);
    const UnkXform* xb = static_cast<const UnkXform*>(tb);
    UnkContactSink* sink = static_cast<UnkContactSink*>(x6);
    UnkContact result = NoContact();
    if (flag)
        result = flag_set(xa, xb, sink);
    CVector3D pa = Translation(xa);
    CVector3D pb = Translation(xb);
    G local = Placer::Placed(a, pa.x - pb.x, pa.y - pb.y);
    UnkContactPoint point{};
    bool found = swapped ? test_reversed(Geometry(b), x5, &local, &point) : test(&local, x5, Geometry(b), &point);
    if (found) {
        CVector3D offset = MakeVec3(xb->m[0][3], xb->m[1][3], (xa->m[2][3] + xb->m[2][3]) * 0.5f);
        result = Merge(result, ReportPoint(sink, &point, &offset));
    }
    return result;
}

// The circle `a` against the type-1 shape `b`; the flag pass places the circle at b's own position.
// NON_MATCHING(minor): the single point's hit is built in another order (the same loads and stores,
// paired differently).
UnkContact fn_7100d54f28(UnkCollider* self, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6,
                         bool swapped, bool flag) {
    return TestSingle<UnkCircle2D, UnkPlacedCircle>(
        a, ta, b, tb, x5, x6, swapped, flag,
        [self, a, b](const UnkXform*, const UnkXform* xb, UnkContactSink* sink) {
            return TestSet<UnkCircle2D, UnkPlacedCircle>(
                self, a, xb, b, xb, sink, 6, [](const UnkCircle2D* p, const void* g, UnkContactBuffer<2>* out) {
                    fn_71001271d0(g, p, out, true);
                });
        },
        fn_710012f260, fn_710012f6a0);
}

// The box `a` against the type-1 shape `b`.
// NON_MATCHING(minor): the single point's hit is built in another order (the same loads and stores,
// paired differently).
UnkContact fn_7100d551c8(UnkCollider* self, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6,
                         bool swapped, bool flag) {
    return TestSingle<UnkBox2D, UnkBox2D>(
        a, ta, b, tb, x5, x6, swapped, flag,
        [self, a, b](const UnkXform* xa, const UnkXform* xb, UnkContactSink* sink) {
            return TestSet<UnkBox2D, UnkBox2D>(self, a, xa, b, xb, sink, 4,
                                               [](const UnkBox2D* p, const void* g, UnkContactBuffer<2>* out) {
                                                   fn_7100128510(p, g, out, true);
                                               });
        },
        [](const UnkBox2D* p, void* x5, const void* g, UnkContactPoint* out) {
            return fn_710012ce70(p, *static_cast<const UnkVec2Result*>(x5), static_cast<const UnkSegment2D*>(g),
                                 reinterpret_cast<UnkContact2D*>(out));
        },
        fn_710012d6b0);
}

bool fn_710012d770(const UnkPlacement2D* a, void* x5, const void* b, UnkContactPoint* out);
bool fn_710012d960(const UnkSegment2D* segment, const base::math::CVector2D* delta, const UnkOrientedBox2D* box,
                   UnkContact2D* out);

// The oriented shape `a` (type 0x40) against the type-1 shape `b`. The single contact point is
// reported to the sink, but its result is dropped: only the flag pass's result is returned.
// NON_MATCHING(minor): the angles temporary shares the point's stack slot instead of the hit's, and
// the hit is built in another order (the same loads and stores, paired differently).
UnkContact fn_7100d55484(UnkCollider* self, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6,
                         bool swapped, bool flag) {
    const UnkXform* xa = static_cast<const UnkXform*>(ta);
    const UnkXform* xb = static_cast<const UnkXform*>(tb);
    UnkOrientedShape* oa = static_cast<UnkOrientedShape*>(a);
    UnkContactSink* sink = static_cast<UnkContactSink*>(x6);
    UnkContact result = NoContact();
    if (flag)
        result = fn_7100d5337c(self, oa, ta, b, tb, sink, 4);
    f32 dx = xa->m[0][3] - xb->m[0][3];
    f32 dy = xa->m[1][3] - xb->m[1][3];
    UnkPlacement2D local = oa->placement;
    local.center.x = dx + local.center.x;
    local.center.y = dy + local.center.y;
    fn_7100110664(&local, fn_7100076acc(xa).z + local.angle);
    UnkContactPoint point{};
    bool found = swapped
                     ? fn_710012d960(
                           static_cast<const UnkSegment2D*>(Geometry(b)), static_cast<const base::math::CVector2D*>(x5),
                           reinterpret_cast<const UnkOrientedBox2D*>(&local), reinterpret_cast<UnkContact2D*>(&point))
                     : fn_710012d770(&local, x5, Geometry(b), &point);
    if (found) {
        CVector3D offset = MakeVec3(xb->m[0][3], xb->m[1][3], (xa->m[2][3] + xb->m[2][3]) * 0.5f);
        UnkContact dropped = sink ? sink->vf_0x10(HitFor(&point, &offset)) : MakeContact(3);
        (void)dropped;
    }
    return result;
}

bool fn_710012b330(const UnkBox2D* a, const void* b, UnkContactBuffer<4>* out, bool flag);
bool fn_710012dbd0(const UnkBox2D* a, void* x5, const void* b, UnkContactPoint* out);

// The box `a` against the box `b`: with `flag`, the point set of the pair first, each point through
// fn_7100d54020 until the sink asks to stop; then the single contact point along `x5`.
UnkContact fn_7100d556d0(UnkNoContext, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6, bool flag) {
    const UnkXform* xa = static_cast<const UnkXform*>(ta);
    const UnkXform* xb = static_cast<const UnkXform*>(tb);
    UnkContactSink* sink = static_cast<UnkContactSink*>(x6);
    UnkContact result = NoContact();
    base::math::CVector2D pa = Position2D(xa);
    base::math::CVector2D pb = Position2D(xb);
    UnkBox2D local = UnkBox2D::Placed(a, pa.x - pb.x, pa.y - pb.y);
    CVector3D offset = Translation(xb);
    offset.z = (xa->m[2][3] + xb->m[2][3]) * 0.5f;
    if (flag) {
        UnkContactBuffer<4> buf;
        buf.count = 0;
        if (fn_710012b330(&local, Geometry(b), &buf, true)) {
            for (u32 i = 0; !(result.value & 2) && i < buf.count; ++i)
                result =
                    Merge(result, fn_7100d54020(UnkNoContext(), buf.points[i], a, ta, b, tb, sink, &offset, x5, 0));
        }
    }
    UnkContactPoint point{};
    if (fn_710012dbd0(&local, x5, Geometry(b), &point))
        result = Merge(result, fn_7100d54020(UnkNoContext(), point, a, ta, b, tb, sink, &offset, x5, 0));
    return result;
}

bool fn_7100127720(const UnkCircle2D* a, const UnkCircle2D& b, UnkContactBuffer<2>* out, bool flag);
bool fn_710012dd70(const UnkCircle2D* a, void* x5, const UnkCircle2D& b, UnkContactPoint* out);

// The circle `a` against the circle `b`, as fn_7100d556d0 (b as a circle at the origin).
// NON_MATCHING(minor): the placed circle's radius is stored after its centre instead of before.
UnkContact fn_7100d55920(UnkNoContext, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6, bool flag) {
    const UnkXform* xa = static_cast<const UnkXform*>(ta);
    const UnkXform* xb = static_cast<const UnkXform*>(tb);
    UnkContactSink* sink = static_cast<UnkContactSink*>(x6);
    const f32* radius = static_cast<const f32*>(Geometry(b));
    UnkContact result = NoContact();
    base::math::CVector2D pa = Position2D(xa);
    base::math::CVector2D pb = Position2D(xb);
    UnkCircle2D local = UnkPlacedCircle::Placed(a, pa.x - pb.x, pa.y - pb.y);
    CVector3D offset = Translation(xb);
    offset.z = (xa->m[2][3] + xb->m[2][3]) * 0.5f;
    if (flag) {
        UnkContactBuffer<2> buf;
        buf.count = 0;
        if (fn_7100127720(&local, CircleAtOrigin(*radius), &buf, true)) {
            for (u32 i = 0; !(result.value & 2) && i < buf.count; ++i)
                result =
                    Merge(result, fn_7100d54020(UnkNoContext(), buf.points[i], a, ta, b, tb, sink, &offset, x5, 0));
        }
    }
    UnkContactPoint point{};
    if (fn_710012dd70(&local, x5, CircleAtOrigin(*radius), &point))
        result = Merge(result, fn_7100d54020(UnkNoContext(), point, a, ta, b, tb, sink, &offset, x5, 0));
    return result;
}

bool fn_7100126bf0(const void* b, const UnkCircle2D* a, UnkContactBuffer<8>* out, bool flag);
bool fn_710012deb0(const UnkCircle2D* circle, const UnkVec2Result* delta, const UnkBox2D* box, UnkContact2D* out);
bool fn_710012e5c0(const UnkBox2D* box, const UnkVec2Result* delta, const UnkCircle2D* circle, UnkContact2D* out);

// The circle `a` against the box `b`: with `flag`, b's point set against the circle first, reported
// the other way round (b first); then the single contact point along `x5`, tested the other way
// round when the pair is swapped.
// NON_MATCHING(minor): the placed circle's radius is stored after its centre instead of before.
UnkContact fn_7100d55b80(UnkNoContext, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6, bool swapped,
                         bool flag) {
    const UnkXform* xa = static_cast<const UnkXform*>(ta);
    const UnkXform* xb = static_cast<const UnkXform*>(tb);
    UnkContactSink* sink = static_cast<UnkContactSink*>(x6);
    UnkContact result = NoContact();
    base::math::CVector2D pa = Position2D(xa);
    base::math::CVector2D pb = Position2D(xb);
    UnkCircle2D local = UnkPlacedCircle::Placed(a, pa.x - pb.x, pa.y - pb.y);
    CVector3D offset = Translation(xb);
    offset.z = (xa->m[2][3] + xb->m[2][3]) * 0.5f;
    if (flag) {
        UnkContactBuffer<8> buf;
        buf.count = 0;
        if (fn_7100126bf0(Geometry(b), &local, &buf, true)) {
            for (u32 i = 0; !(result.value & 2) && i < buf.count; ++i)
                result =
                    Merge(result, fn_7100d54020(UnkNoContext(), buf.points[i], b, tb, a, ta, sink, &offset, x5, 2));
        }
    }
    UnkContactPoint point{};
    const UnkBox2D* box = static_cast<const UnkBox2D*>(Geometry(b));
    const UnkVec2Result* delta = static_cast<const UnkVec2Result*>(x5);
    UnkContact2D* out = reinterpret_cast<UnkContact2D*>(&point);
    bool found = swapped ? fn_710012e5c0(box, delta, &local, out) : fn_710012deb0(&local, delta, box, out);
    if (found)
        result = Merge(result, fn_7100d54020(UnkNoContext(), point, a, ta, b, tb, sink, &offset, x5, 0));
    return result;
}

bool fn_710012e400(const UnkCircle2D& b, void* x5, const UnkPlacement2D* a, UnkContactPoint* out);

// The oriented shape `a` (type 0x40) against the circle `b`: with `flag`, the point set of the pair
// (fn_7100128da0) first; then, unless the pair is swapped, the single contact point along `x5`.
UnkContact fn_7100d55df4(UnkNoContext, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6, bool swapped,
                         bool flag) {
    const UnkXform* xa = static_cast<const UnkXform*>(ta);
    const UnkXform* xb = static_cast<const UnkXform*>(tb);
    UnkOrientedShape* oa = static_cast<UnkOrientedShape*>(a);
    UnkContactSink* sink = static_cast<UnkContactSink*>(x6);
    const f32* radius = static_cast<const f32*>(Geometry(b));
    UnkContact result = NoContact();
    f32 dx = xa->m[0][3] - xb->m[0][3];
    f32 dy = xa->m[1][3] - xb->m[1][3];
    UnkPlacement2D local = oa->placement;
    local.center.x = dx + local.center.x;
    local.center.y = dy + local.center.y;
    fn_7100110664(&local, fn_7100076acc(xa).z + local.angle);
    CVector3D offset = Translation(xb);
    offset.z = (xa->m[2][3] + xb->m[2][3]) * 0.5f;
    if (flag) {
        UnkContactBuffer<8> buf;
        buf.count = 0;
        if (fn_7100128da0(&local, CircleAtOrigin(*radius), &buf, true)) {
            for (u32 i = 0; !(result.value & 2) && i < buf.count; ++i)
                result =
                    Merge(result, fn_7100d54020(UnkNoContext(), buf.points[i], a, ta, b, tb, sink, &offset, x5, 0));
        }
    }
    UnkContactPoint point{};
    if (!swapped && fn_710012e400(CircleAtOrigin(*radius), x5, &local, &point))
        result = Merge(result, fn_7100d54020(UnkNoContext(), point, a, ta, b, tb, sink, &offset, x5, 0));
    return result;
}

bool fn_7100129ff0(const UnkBox2D* b, const UnkShape8Geometry* a, UnkContactBuffer<4>* out, bool flag);
bool fn_710012ed30(const UnkShape8Geometry* a, void* x5, const void* b, UnkContactPoint* out);
bool fn_710012efb0(const void* b, void* x5, const UnkShape8Geometry* a, UnkContactPoint* out);

// The type-8 shape `a` against the box `b`: with `flag`, the box placed in a's frame against a's own
// geometry first, reported the other way round (b first); then the single contact point along `x5`,
// tested the other way round when the pair is swapped.
UnkContact fn_7100d560bc(UnkNoContext, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6, bool swapped,
                         bool flag) {
    const UnkXform* xa = static_cast<const UnkXform*>(ta);
    const UnkXform* xb = static_cast<const UnkXform*>(tb);
    UnkContactSink* sink = static_cast<UnkContactSink*>(x6);
    UnkContact result = NoContact();
    base::math::CVector2D pa = Position2D(xa);
    base::math::CVector2D pb = Position2D(xb);
    f32 dx = pa.x - pb.x;
    f32 dy = pa.y - pb.y;
    UnkShape8Geometry local = UnkShape8Geometry::Placed(a, dx, dy);
    CVector3D offset = Translation(xb);
    offset.z = (xa->m[2][3] + xb->m[2][3]) * 0.5f;
    if (flag) {
        UnkContactBuffer<4> buf;
        buf.count = 0;
        const UnkBox2D* g = static_cast<const UnkBox2D*>(Geometry(b));
        UnkBox2D other = UnkBox2D::Make(g->min_x - dx, g->min_y - dy, g->max_x - dx, g->max_y - dy);
        if (fn_7100129ff0(&other, static_cast<const UnkShape8Geometry*>(Geometry(a)), &buf, true)) {
            for (u32 i = 0; !(result.value & 2) && i < buf.count; ++i)
                result =
                    Merge(result, fn_7100d54020(UnkNoContext(), buf.points[i], b, tb, a, ta, sink, &offset, x5, 2));
        }
    }
    UnkContactPoint point{};
    bool found =
        swapped ? fn_710012efb0(Geometry(b), x5, &local, &point) : fn_710012ed30(&local, x5, Geometry(b), &point);
    if (found)
        result = Merge(result, fn_7100d54020(UnkNoContext(), point, a, ta, b, tb, sink, &offset, x5, 0));
    return result;
}

bool fn_710012a9a0(const UnkCircle2D* b, const UnkShape8Geometry* a, UnkContactBuffer<4>* out, bool flag);
bool fn_710012eb40(const UnkShape8Geometry* a, void* x5, const UnkCircle2D& b, UnkContactPoint* out);
bool fn_710012ec40(const UnkCircle2D& b, void* x5, const UnkShape8Geometry* a, UnkContactPoint* out);

// The type-8 shape `a` against the circle `b`, as fn_7100d560bc.
UnkContact fn_7100d5635c(UnkNoContext, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6, bool swapped,
                         bool flag) {
    const UnkXform* xa = static_cast<const UnkXform*>(ta);
    const UnkXform* xb = static_cast<const UnkXform*>(tb);
    UnkContactSink* sink = static_cast<UnkContactSink*>(x6);
    const f32* radius = static_cast<const f32*>(Geometry(b));
    UnkContact result = NoContact();
    base::math::CVector2D pa = Position2D(xa);
    base::math::CVector2D pb = Position2D(xb);
    f32 dx = pa.x - pb.x;
    f32 dy = pa.y - pb.y;
    UnkShape8Geometry local = UnkShape8Geometry::Placed(a, dx, dy);
    CVector3D offset = Translation(xb);
    offset.z = (xa->m[2][3] + xb->m[2][3]) * 0.5f;
    if (flag) {
        UnkContactBuffer<4> buf;
        buf.count = 0;
        UnkCircle2D other = CircleAtOrigin(*radius);
        other.x = other.x - dx;
        other.y = other.y - dy;
        if (fn_710012a9a0(&other, static_cast<const UnkShape8Geometry*>(Geometry(a)), &buf, true)) {
            for (u32 i = 0; !(result.value & 2) && i < buf.count; ++i)
                result =
                    Merge(result, fn_7100d54020(UnkNoContext(), buf.points[i], b, tb, a, ta, sink, &offset, x5, 2));
        }
    }
    UnkContactPoint point{};
    bool found = swapped ? fn_710012ec40(CircleAtOrigin(*radius), x5, &local, &point)
                         : fn_710012eb40(&local, x5, CircleAtOrigin(*radius), &point);
    if (found)
        result = Merge(result, fn_7100d54020(UnkNoContext(), point, a, ta, b, tb, sink, &offset, x5, 0));
    return result;
}
