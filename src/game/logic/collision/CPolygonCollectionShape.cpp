// game::logic::collision::CPolygonCollectionShape's unit (its reflection is
// src/reflection/game_logic_collision_CPolygonCollectionShape.cpp): the queries of a polygon
// collection shape (type 0x10) against a shape placed in its frame. With a tree, the query walks
// its nodes (a box each, skip indices for the misses) and hands every leaf it reaches to a leaf
// visitor; without one, it tests every edge of every polygon. Hits go to the query's sink
// (UnkLocalQuery::vf_0x10), one at a time, until it asks to stop (bit 1 of the result).
// Holds the tree walks fn_7100d636d8 (segment), fn_7100d63870 (box), fn_7100d63c74 (type-8
// shape), fn_7100d64088 (circle), fn_7100d64d3c, fn_7100d65200 and fn_7100d656a0 (circle, box and
// type-8 shape moved along a direction) and fn_7100d65b70 (query box), and the edge loops
// fn_7100d644d0, fn_7100d64f70, fn_7100d65410 and fn_7100d658e0. Names and layouts are
// placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// The result of a query (as in src/game/UnkCollisionContacts.cpp): bit 0 a hit kept, bit 1 stop.
// Returned in memory: its copy is user-written.
struct UnkContact {
    u32 value;

    UnkContact() { value = 0; }
    UnkContact(const UnkContact& other) { value = other.value; }
};

inline const UnkContact& NoContact() {
    static UnkContact s_none;
    return s_none;
}

// The contact a pair test fills (as in src/game/UnkCollisionContacts.cpp): the point, the two
// shapes' normals and flags.
struct UnkContactPoint {
    CVector3D point;
    CVector3D normal_a;
    CVector3D normal_b;
    u32 field_0x24;
    u32 flags;
};
static_assert(sizeof(UnkContactPoint) == 0x2c, "UnkContactPoint layout");

// The same contact under the names the pair tests' own sources give it (their signatures).
struct UnkContact_710012be70 : UnkContactPoint {};
struct UnkContact2D : UnkContactPoint {};

// The pair tests against one edge: a segment (fn_710012be70), and a circle, a box and a type-8 shape
// moving by a step (fn_710012f260, fn_710012ce70, fn_710012f750).
bool fn_710012be70(const UnkSegment2D* a, const UnkSegment2D* b, UnkContact_710012be70* out, bool collinear,
                   f32 tolerance);

// A polygon's vertex (as in src/base/spatial/CPolygonCollection2D.cpp).
struct UnkPolygonVertex {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkPolygon2D {
    UnkPolygonVertex* vertices;
    u32 count;
    u8 unk_0xc[0x20];
    u8 closed;
    u8 unk_0x2d[0x1b];
};
static_assert(sizeof(UnkPolygon2D) == 0x48, "UnkPolygon2D layout");

// The number of edges: an open polygon has none from its last vertex back to the first.
static inline u32 EdgeCount(const UnkPolygon2D& p) {
    return p.count - (!p.closed & (p.count != 0));
}

struct UnkPolygonCollection2D {
    UnkPolygon2D* polygons;
    u32 count;
};

// What the sink gets for a hit: the polygon, the edge's handle (the polygon's index in bits 16 to
// 30, the edge's in the low bits) and the contact.
struct UnkPolygonHit {
    const UnkPolygon2D* polygon;
    u32 handle;
    UnkContactPoint point;
};
static_assert(sizeof(UnkPolygonHit) == 0x38, "UnkPolygonHit layout");

// The query a polygon collection shape runs (same type as in src/game/UnkCollisionContacts.cpp and
// src/game_hi/fn_7100d56604.cpp, vtable at 0x7101aa8870).
struct UnkLocalQuery {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkContact vf_0x10(UnkPolygonHit hit);
    CVector3D offset;
    u16 field_0x14;
    const void* field_0x18;
    void* field_0x20;
};

// A tree node: its box and, at 0x10, where the walk goes on when the box misses (< 0 for a leaf,
// which is followed by the next node).
struct UnkTreeNode {
    f32 min_x, min_y;
    f32 max_x, max_y;
    s32 next;
};
static_assert(sizeof(UnkTreeNode) == 0x14, "UnkTreeNode layout");

// The base shape (0xa8 bytes); the shape data follows it.
struct UnkShape {
    u8 unk_0x0[0xa8];
};

// The polygon collection shape (type 0x10).
struct UnkShape10 : UnkShape {
    UnkPolygonCollection2D polygons;
    u8 unk_0xb8[0x30];
    bool has_tree;
    UnkTreeNode* nodes;
    u32 node_count;
};
static_assert(__builtin_offsetof(UnkShape10, has_tree) == 0xe8, "UnkShape10 layout");
static_assert(__builtin_offsetof(UnkShape10, nodes) == 0xf0, "UnkShape10 layout");

// The query shapes, in the collection's frame (as in src/game/UnkCollisionContacts.cpp).
// An axis-aligned box (type 4).
struct UnkBox2D {
    f32 min_x, min_y;
    f32 max_x, max_y;
};

// A circle (type 2).
struct UnkCircle2D {
    f32 x, y;
    f32 radius;
};

// A type-8 shape: a position, a half length along y and a radius.
struct UnkShape8Geometry {
    f32 x, y;
    f32 field_0x8;
    f32 field_0xc;
};

// A query box (as in src/game_hi/fn_7100d56604.cpp): two corners in any order, two more words.
struct UnkQueryBox {
    f32 minX, minY, maxX, maxY;
    u32 field_0x10;
    u32 field_0x14;
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

struct UnkBox3D {
    CVector3D min;
    CVector3D max;
};

static inline UnkBox3D MakeBox3D(const CVector3D& min, const CVector3D& max) {
    UnkBox3D b;
    b.min = min;
    b.max = max;
    return b;
}

// The tree's 2D box of a 3D one (fn_710007b430 drops each corner's z).
struct UnkTreeBox {
    CVector2D min;
    CVector2D max;

    explicit UnkTreeBox(const UnkBox3D& b) {
        fn_710007b430(&min, &b.min);
        fn_710007b430(&max, &b.max);
    }
};

// A segment's bounding box.
UnkBox3D fn_7100126ba0(const UnkSegment2D* seg);

// A 3D vector's x and y (fn_710007b430), as a temporary.
struct UnkProjected : CVector2D {
    explicit UnkProjected(const CVector3D* v) { fn_710007b430(this, v); }
};

// Visitor arguments are copied member by member.
static inline bool CopyOf(bool b) {
    return b;
}
static inline CVector2D CopyOf(const CVector2D& v) {
    CVector2D r;
    r.x = v.x;
    r.y = v.y;
    return r;
}

// What a leaf visitor carries: the query shape, one more argument, the query and the result so far.
template <class G, class E>
struct UnkLeafVisitor {
    const G* shape;
    E extra;
    UnkLocalQuery* query;
    UnkContact result;

    UnkLeafVisitor(const G* s, const E& e, UnkLocalQuery* q)
        : shape(s), extra(CopyOf(e)), query(q), result(NoContact()) {}
};

// Walks the tree: a node whose box meets `box` is entered (a leaf goes to `leaf`, which returns
// false to stop), a node that misses is skipped.
template <class V>
static inline void WalkTree(const UnkShape10* self, const UnkTreeBox& box, V* v, bool (*leaf)(V*, const UnkTreeNode*)) {
    u32 count = self->node_count;
    u32 i = 0;
    while (i < count) {
        const UnkTreeNode& n = self->nodes[i];
        if (box.max.x >= n.min_x && box.min.x <= n.max_x && box.max.y >= n.min_y && box.min.y <= n.max_y) {
            if (n.next < 0 && !leaf(v, &n))
                break;
            ++i;
        } else {
            i = n.next >= 0 ? (u32)n.next : i + 1;
        }
    }
}

bool fn_7100d66bc4(UnkLeafVisitor<UnkSegment2D, bool>* self, const UnkTreeNode* node);
UnkContact fn_7100d644d0(UnkPolygonCollection2D* self, const UnkSegment2D* seg, UnkLocalQuery* query);

// A segment against the collection.
UnkContact fn_7100d636d8(UnkShape10* self, const UnkSegment2D* seg, UnkLocalQuery* query) {
    if (!self->has_tree)
        return fn_7100d644d0(&self->polygons, seg, query);
    UnkLeafVisitor<UnkSegment2D, bool> v(seg, false, query);
    UnkTreeBox box(fn_7100126ba0(seg));
    WalkTree(self, box, &v, fn_7100d66bc4);
    return v.result;
}

// A segment against every edge of every polygon; with a sink, each hit is reported with its
// polygon and handle, without one the first hit stops the query.
// NON_MATCHING(minor): register allocation and layout only: the polygon loop's latch is placed
// before its body, the edge loop counts the byte offset of the edge's end vertex instead of its
// start, and the hit's contact is copied through a base register hoisted out of the loops (more
// spills).
UnkContact fn_7100d644d0(UnkPolygonCollection2D* self, const UnkSegment2D* seg, UnkLocalQuery* query) {
    UnkContact result = NoContact();
    u64 base = 0;
    for (const UnkPolygon2D *p = self->polygons, *end = p + self->count; p != end && !(result.value & 2);
         ++p, base += 0x10000) {
        for (u64 i = 0; !(result.value & 2) && i < EdgeCount(*p); ++i) {
            const UnkPolygonVertex* v = &p->vertices[i];
            UnkSegment2D edge(v->pos, (i == p->count - 1 ? p->vertices : v + 1)->pos);
            UnkContact_710012be70 point = {};
            if (fn_710012be70(seg, &edge, &point, false, 0.0f)) {
                if (query)
                    result.value |= query->vf_0x10(UnkPolygonHit{p, (u32)(base + i), point}).value;
                else
                    result.value |= 3;
            }
        }
    }
    return result;
}

static const f32 kFloatMax = 3.4028235e38f;

// A shape's box, unbounded in z.
static inline UnkBox3D Unbounded(f32 min_x, f32 min_y, f32 max_x, f32 max_y) {
    return MakeBox3D(MakeVec3(min_x, min_y, -kFloatMax), MakeVec3(max_x, max_y, kFloatMax));
}

// The box covering two others.
static inline UnkBox2D Union(const UnkBox2D& a, const UnkBox2D& b) {
    UnkBox2D r;
    r.min_x = a.min_x < b.min_x ? a.min_x : b.min_x;
    r.min_y = a.min_y < b.min_y ? a.min_y : b.min_y;
    r.max_x = a.max_x > b.max_x ? a.max_x : b.max_x;
    r.max_y = a.max_y > b.max_y ? a.max_y : b.max_y;
    return r;
}

bool fn_7100d6675c(UnkLeafVisitor<UnkBox2D, bool>* self, const UnkTreeNode* node);
UnkContact fn_7100d63a40(UnkPolygonCollection2D* self, const UnkBox2D* box, bool flag, UnkLocalQuery* query);

// A box against the collection.
UnkContact fn_7100d63870(UnkShape10* self, const UnkBox2D* box, UnkLocalQuery* query, bool flag) {
    if (!self->has_tree)
        return fn_7100d63a40(&self->polygons, box, !(query->field_0x14 & 1), query);
    UnkLeafVisitor<UnkBox2D, bool> v(box, flag, query);
    UnkTreeBox tree_box(Unbounded(box->min_x, box->min_y, box->max_x, box->max_y));
    WalkTree(self, tree_box, &v, fn_7100d6675c);
    return v.result;
}

bool fn_7100d668d4(UnkLeafVisitor<UnkShape8Geometry, bool>* self, const UnkTreeNode* node);
UnkContact fn_7100d63e50(UnkPolygonCollection2D* self, const UnkShape8Geometry* shape, bool flag,
                         UnkLocalQuery* query);

// A type-8 shape against the collection (its box is flat in z).
UnkContact fn_7100d63c74(UnkShape10* self, const UnkShape8Geometry* shape, UnkLocalQuery* query, bool flag) {
    if (!self->has_tree)
        return fn_7100d63e50(&self->polygons, shape, !(query->field_0x14 & 1), query);
    UnkLeafVisitor<UnkShape8Geometry, bool> v(shape, flag, query);
    f32 r = shape->field_0xc;
    f32 e = r + shape->field_0x8;
    UnkTreeBox tree_box(MakeBox3D(MakeVec3(shape->x - r, shape->y - e, 0.0f), MakeVec3(r + shape->x, e + shape->y, 0.0f)));
    WalkTree(self, tree_box, &v, fn_7100d668d4);
    return v.result;
}

bool fn_7100d66a4c(UnkLeafVisitor<UnkCircle2D, bool>* self, const UnkTreeNode* node);
UnkContact fn_7100d64270(UnkPolygonCollection2D* self, const UnkCircle2D* circle, bool flag,
                         UnkLocalQuery* query);

// A circle against the collection.
UnkContact fn_7100d64088(UnkShape10* self, const UnkCircle2D* circle, UnkLocalQuery* query, bool flag) {
    if (!self->has_tree)
        return fn_7100d64270(&self->polygons, circle, !(query->field_0x14 & 1), query);
    UnkLeafVisitor<UnkCircle2D, bool> v(circle, flag, query);
    f32 x = circle->x;
    f32 y = circle->y;
    f32 h = (circle->radius + circle->radius) * 0.5f;
    UnkTreeBox tree_box(Unbounded(x - h, y - h, x + h, y + h));
    WalkTree(self, tree_box, &v, fn_7100d66a4c);
    return v.result;
}

bool fn_7100d66d84(UnkLeafVisitor<UnkCircle2D, CVector2D>* self, const UnkTreeNode* node);
UnkContact fn_7100d64f70(UnkPolygonCollection2D* self, const UnkCircle2D* circle, const CVector3D* dir,
                         UnkLocalQuery* query);

static inline UnkBox2D CircleBox(f32 x, f32 y, f32 r) {
    UnkBox2D b = {x - r, y - r, r + x, r + y};
    return b;
}

// A circle moving along `dir` against the collection: the tree is walked with the box covering
// both ends of the move.
UnkContact fn_7100d64d3c(UnkShape* self, const UnkCircle2D* circle, const CVector3D* dir, UnkLocalQuery* query) {
    UnkShape10* shape = static_cast<UnkShape10*>(self);
    if (!shape->has_tree)
        return fn_7100d64f70(&shape->polygons, circle, dir, query);
    f32 r = circle->radius;
    UnkBox2D swept = Union(CircleBox(circle->x, circle->y, r), CircleBox(circle->x + dir->x, circle->y + dir->y, r));
    UnkLeafVisitor<UnkCircle2D, CVector2D> v(circle, UnkProjected(dir), query);
    UnkTreeBox tree_box(Unbounded(swept.min_x, swept.min_y, swept.max_x, swept.max_y));
    WalkTree(shape, tree_box, &v, fn_7100d66d84);
    return v.result;
}

bool fn_7100d66ed8(UnkLeafVisitor<UnkBox2D, CVector2D>* self, const UnkTreeNode* node);
UnkContact fn_7100d65410(UnkPolygonCollection2D* self, const UnkBox2D* box, const CVector3D* dir,
                         UnkLocalQuery* query);

// A box moving along `dir` against the collection.
UnkContact fn_7100d65200(UnkShape* self, const UnkBox2D* box, const CVector3D* dir, UnkLocalQuery* query) {
    UnkShape10* shape = static_cast<UnkShape10*>(self);
    if (!shape->has_tree)
        return fn_7100d65410(&shape->polygons, box, dir, query);
    UnkBox2D moved = {dir->x + box->min_x, dir->y + box->min_y, dir->x + box->max_x, dir->y + box->max_y};
    UnkBox2D swept = Union(moved, *box);
    UnkLeafVisitor<UnkBox2D, CVector2D> v(box, UnkProjected(dir), query);
    UnkTreeBox tree_box(Unbounded(swept.min_x, swept.min_y, swept.max_x, swept.max_y));
    WalkTree(shape, tree_box, &v, fn_7100d66ed8);
    return v.result;
}

bool fn_7100d6702c(UnkLeafVisitor<UnkShape8Geometry, CVector2D>* self, const UnkTreeNode* node);
UnkContact fn_7100d658e0(UnkPolygonCollection2D* self, const UnkShape8Geometry* shape, const CVector3D* dir,
                         UnkLocalQuery* query);

static inline UnkBox2D CenteredBox(f32 x, f32 y, f32 hx, f32 hy) {
    UnkBox2D b = {x - hx, y - hy, x + hx, y + hy};
    return b;
}

// A type-8 shape moving along `dir` against the collection.
// NON_MATCHING(minor): the swept box's half sizes and the shape's position get other float registers
// and the direction is loaded earlier (same operations in the same order otherwise).
UnkContact fn_7100d656a0(UnkShape* self, const UnkShape8Geometry* geom, const CVector3D* dir, UnkLocalQuery* query) {
    UnkShape10* shape = static_cast<UnkShape10*>(self);
    if (!shape->has_tree)
        return fn_7100d658e0(&shape->polygons, geom, dir, query);
    f32 r = geom->field_0xc;
    f32 e = r + geom->field_0x8;
    f32 sx = r + r;
    f32 sy = e + e;
    f32 hx = sx * 0.5f;
    f32 hy = sy * 0.5f;
    UnkBox2D swept =
        Union(CenteredBox(geom->x, geom->y, hx, hy), CenteredBox(geom->x + dir->x, geom->y + dir->y, hx, hy));
    UnkLeafVisitor<UnkShape8Geometry, CVector2D> v(geom, UnkProjected(dir), query);
    UnkTreeBox tree_box(Unbounded(swept.min_x, swept.min_y, swept.max_x, swept.max_y));
    WalkTree(shape, tree_box, &v, fn_7100d6702c);
    return v.result;
}

// What the query-box visitor carries.
struct UnkBoxVisitor {
    const UnkQueryBox* box;
    UnkLocalQuery* query;
    UnkContact result;

    UnkBoxVisitor(const UnkQueryBox* b, UnkLocalQuery* q) : box(b), query(q), result(NoContact()) {}
};

bool fn_7100d67180(UnkBoxVisitor* self, const UnkTreeNode* node);
UnkContact fn_7100d65d50(UnkPolygonCollection2D* self, const UnkQueryBox* box, UnkLocalQuery* query);

// A query box (its corners in any order) against the collection.
UnkContact fn_7100d65b70(UnkShape10* self, const UnkQueryBox* box, UnkLocalQuery* query) {
    if (!self->has_tree)
        return fn_7100d65d50(&self->polygons, box, query);
    f32 x0 = box->minX, y0 = box->minY;
    f32 x1 = box->maxX, y1 = box->maxY;
    f32 lo_x = x1 > x0 ? x0 : x1;
    f32 hi_x = x1 < x0 ? x0 : x1;
    f32 lo_y = y1 > y0 ? y0 : y1;
    f32 hi_y = y1 < y0 ? y0 : y1;
    UnkBoxVisitor v(box, query);
    UnkTreeBox tree_box(Unbounded(lo_x, lo_y, hi_x, hi_y));
    WalkTree(self, tree_box, &v, fn_7100d67180);
    return v.result;
}

// A probe for fn_71001244ac (as in src/base/spatial/CPolygonCollection2D.cpp): a contact's point and
// first normal.
struct UnkPolygonProbe;

// Whether the contact at the edge `handle` is a real one (at a corner, whether it points into it).
bool fn_71001244ac(UnkPolygonCollection2D* self, u32 handle, const UnkPolygonProbe* probe);

// A moving shape against every edge of every polygon: like fn_7100d644d0, but a contact `Test` finds
// also has to pass fn_71001244ac.
template <class Shape, class Point, class Test>
static inline UnkContact SweepEdges(UnkPolygonCollection2D* self, const Shape* shape, const CVector3D* dir,
                                    UnkLocalQuery* query, Test test) {
    UnkContact result = NoContact();
    UnkVec2Result step(dir->x, dir->y);
    u64 base = 0;
    for (const UnkPolygon2D *p = self->polygons, *end = p + self->count; p != end && !(result.value & 2);
         ++p, base += 0x10000) {
        for (u64 i = 0; !(result.value & 2) && i < EdgeCount(*p); ++i) {
            const UnkPolygonVertex* v = &p->vertices[i];
            UnkSegment2D edge(v->pos, (i == p->count - 1 ? p->vertices : v + 1)->pos);
            Point point = {};
            if (test(shape, step, &edge, &point)) {
                u32 handle = (u32)(base + i);
                if (fn_71001244ac(self, handle, reinterpret_cast<const UnkPolygonProbe*>(&point))) {
                    if (query)
                        result.value |= query->vf_0x10(UnkPolygonHit{p, handle, point}).value;
                    else
                        result.value |= 3;
                }
            }
        }
    }
    return result;
}

bool fn_710012f260(const UnkCircle2D* a, void* x5, const void* b, UnkContactPoint* out);
bool fn_710012ce70(const UnkBox2D* box, const UnkVec2Result& step, const UnkSegment2D* seg, UnkContact2D* out);
bool fn_710012f750(const UnkShape8Geometry* shape, const UnkVec2Result& step, const UnkSegment2D* seg,
                   UnkContactPoint* out);

// NON_MATCHING(minor): register allocation and layout only: the polygon loop's latch is placed
// before its body, the edge loop counts the byte offset of the edge's end vertex instead of its
// start, and the hit's contact is copied through a base register hoisted out of the loops (more
// spills).
UnkContact fn_7100d64f70(UnkPolygonCollection2D* self, const UnkCircle2D* circle, const CVector3D* dir,
                         UnkLocalQuery* query) {
    return SweepEdges<UnkCircle2D, UnkContactPoint>(
        self, circle, dir, query,
        [](const UnkCircle2D* c, UnkVec2Result& step, const UnkSegment2D* edge, UnkContactPoint* out) {
            return fn_710012f260(c, &step, edge, out);
        });
}

// NON_MATCHING(minor): register allocation and layout only: the polygon loop's latch is placed
// before its body, the edge loop counts the byte offset of the edge's end vertex instead of its
// start, and the hit's contact is copied through a base register hoisted out of the loops (more
// spills).
UnkContact fn_7100d65410(UnkPolygonCollection2D* self, const UnkBox2D* box, const CVector3D* dir,
                         UnkLocalQuery* query) {
    return SweepEdges<UnkBox2D, UnkContact2D>(self, box, dir, query, fn_710012ce70);
}

// NON_MATCHING(minor): register allocation and layout only: the polygon loop's latch is placed
// before its body, the edge loop counts the byte offset of the edge's end vertex instead of its
// start, and the hit's contact is copied through a base register hoisted out of the loops (more
// spills).
UnkContact fn_7100d658e0(UnkPolygonCollection2D* self, const UnkShape8Geometry* shape, const CVector3D* dir,
                         UnkLocalQuery* query) {
    return SweepEdges<UnkShape8Geometry, UnkContactPoint>(self, shape, dir, query, fn_710012f750);
}
