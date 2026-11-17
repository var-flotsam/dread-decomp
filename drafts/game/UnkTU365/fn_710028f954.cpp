// Unit 365 (navmesh::CNavMeshGenerator): splits each polygon's edges where the source edges touch or
// cross them, snapping crossings to the grid, and keeps the refined polygons that gained edges.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkTU108.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkContact_710012be70;
bool fn_710012be70(const UnkSegment2D* a, const UnkSegment2D* b, UnkContact_710012be70* out, bool collinear,
                   f32 tolerance);
void* fn_7100118950(void* self, u32 index);

extern const f32 g_unk_7101647774;  // the tolerance two points are equal within
extern const f32 g_unk_7101648c88;  // the grid cell crossings snap to

// A polygon vertex: its plane position and a height.
struct UnkNavVertex_28f954 {
    CVector2D xy;
    f32 z;
};

// A polygon's vertex array (base::global::CRntVector's layout).
struct UnkNavVertexArray_28f954 {
    UnkNavVertex_28f954* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    explicit UnkNavVertexArray_28f954(u32 flags)
        : data(nullptr), count(0), capacity(0), step(10), field_0x14(true), flags(flags),
          allocator(fn_7100083990()) {}
    // Out of line: takes the other's settings and copies its vertices.
    UnkNavVertexArray_28f954(const UnkNavVertexArray_28f954& other) {
        fn_710011d8b4(reinterpret_cast<UnkPart0*>(this), reinterpret_cast<const UnkPart0*>(&other));
    }
    ~UnkNavVertexArray_28f954() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

// A 0x48-byte polygon: its vertices, then the cached length, flags, bounds and winding state.
struct UnkNavPolygon_28f954 {
    UnkNavVertexArray_28f954 vertices;
    f32 field_0x28;
    u8 closed;    // 0x2c
    bool dirty;   // 0x2d
    CVector2D bounds_min;  // 0x30
    CVector2D bounds_max;  // 0x38
    u32 state;             // 0x40

    UnkNavPolygon_28f954() : vertices(7) {
        field_0x28 = 0.0f;
        closed = 0;
        dirty = true;
        bounds_min.x = 3.40282347e38f;
        bounds_min.y = 3.40282347e38f;
        bounds_max.x = -3.40282347e38f;
        bounds_max.y = -3.40282347e38f;
        state = 0;
    }

    UnkNavPolygon_28f954(const UnkNavPolygon_28f954& other) : vertices(other.vertices) {
        field_0x28 = other.field_0x28;
        closed = other.closed;
        dirty = true;
        bounds_min.x = 3.40282347e38f;
        bounds_min.y = 3.40282347e38f;
        bounds_max.x = -3.40282347e38f;
        bounds_max.y = -3.40282347e38f;
        state = other.state;
    }

    // An open polygon has one edge fewer than vertices.
    u32 EdgeCount() const { return vertices.count - (closed == 0 && vertices.count != 0); }

    bool IsLastEdge(u64 i) const { return vertices.count > 1 && i == EdgeCount() - 1; }

    bool BoundsContain(const CVector2D& p) const {
        return p.x >= bounds_min.x && p.x <= bounds_max.x && p.y >= bounds_min.y && p.y <= bounds_max.y;
    }
};

// The polygons the generator keeps, with a flag set when one is added.
struct UnkNavPolygonList_28f954 {
    UnkNavPolygon_28f954* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
    u8 pad_0x28[0x10];
    bool changed;  // 0x38

    // Out of line: grows the array to at least n elements.
    void Reserve(u32 n) asm("fn_710011f1ec");
};

// Appends a copy of the polygon.
static void PushBackCopy(UnkNavPolygonList_28f954* self, const UnkNavPolygon_28f954& value) {
    u32 n = self->count;
    u32 cap = self->capacity;
    u32 new_capacity = cap;
    if (cap <= n && self->step != 0)
        new_capacity = (n / self->step + 1) * self->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    self->Reserve(new_capacity);
    new (&self->data[self->count]) UnkNavPolygon_28f954(value);
    ++self->count;
}

// A split point, copied member by member.
struct UnkNavPoint_28f954 {
    f32 x;
    f32 y;

    UnkNavPoint_28f954(const CVector2D& v) : x(v.x), y(v.y) {}
    UnkNavPoint_28f954(const UnkNavPoint_28f954& other) : x(other.x), y(other.y) {}
};

// The split points found, for the caller.
struct UnkNavPointArray_28f954 {
    UnkNavPoint_28f954* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    void PushBack(const CVector2D& value) {
        u32 n = count;
        u32 cap = capacity;
        u32 new_capacity = cap;
        if (cap <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        UnkNavPoint_28f954* p;
        if (cap < new_capacity) {
            p = static_cast<UnkNavPoint_28f954*>(
                allocator->vf_0x40((u64)new_capacity * sizeof(UnkNavPoint_28f954), 8, flags));
            if (data) {
                UnkNavPoint_28f954* src = data;
                UnkNavPoint_28f954* dst = p;
                for (u32 i = 0; i < count; ++i, ++dst, ++src)
                    new (dst) UnkNavPoint_28f954(*src);
                allocator->vf_0x48(data);
            }
            n = count;
            data = p;
            capacity = new_capacity;
        } else {
            p = data;
        }
        new (&p[n]) UnkNavPoint_28f954(value);
        ++count;
    }
};

// A source edge: two vertices, then the next edge.
struct UnkNavEdge_28f954 {
    u8 pad_0x0[0xc];
    CVector2D a;  // 0xc
    u8 pad_0x14[0x10];
    CVector2D b;  // 0x24
    u8 pad_0x2c[4];
    UnkNavEdge_28f954* next;  // 0x30
};

struct UnkNavEdgeSet_28f954 {
    u8 pad_0x0[0x10];
    UnkNavEdge_28f954* head;
};

struct UnkNavShape_28f954 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();
    u8 pad_0x8[0xa0];
    base::global::CRntVector<UnkNavPolygon_28f954> polygons;  // 0xa8
};

struct UnkNavSource_28f954 {
    u8 pad_0x0[0x158];
    UnkNavShape_28f954* shape;
};

struct UnkNavMeshGenerator_28f954;

static inline bool NearEps(f32 a, f32 b, f32 eps) {
    f32 hi = eps + a;
    f32 lo = a - eps;
    return hi >= b && lo <= b;
}

static inline bool Near1(f32 a, f32 b) { return a - 1.0f <= b && a + 1.0f >= b; }

template <class A, class B>
static inline bool Near1(const A& a, const B& b) { return Near1(a.x, b.x) && Near1(a.y, b.y); }

template <class A, class B>
static inline f32 DistSq(const A& a, const B& b) {
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;
    return dx * dx + dy * dy;
}

// A coordinate rounded to the nearest multiple of the cell, away from zero on a tie.
static inline f32 Snap(f32 v, f32 cell, f32 half) {
    f32 a = __builtin_fabsf(v);
    f32 s = cell * (f32)(s32)(a / cell);
    f32 r = a - s > half ? cell + s : s;
    return r * (f32)((v > 0.0f) - (v < 0.0f));
}

struct UnkPointNode_28f954 {
    CVector2D p;
    UnkPointNode_28f954* next;
};

// The points on one edge, ordered from its start to its end.
struct UnkPointList_28f954 {
    UnkPointNode_28f954* head = nullptr;
    UnkPointNode_28f954* tail = nullptr;
    u32 count = 0;
    base::UnkAllocator* allocator = fn_7100083990();

    ~UnkPointList_28f954() {
        for (UnkPointNode_28f954* n = head; n;) {
            UnkPointNode_28f954* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
    }

    bool Contains(const CVector2D& p, f32 eps) const {
        for (UnkPointNode_28f954* n = head; n; n = n->next)
            if (NearEps(n->p.x, p.x, eps) && NearEps(n->p.y, p.y, eps))
                return true;
        return false;
    }

    s32 Find(const CVector2D& p, f32 eps) const {
        s32 i = 0;
        for (UnkPointNode_28f954* n = head; n; n = n->next, ++i)
            if (NearEps(n->p.x, p.x, eps) && NearEps(n->p.y, p.y, eps))
                return i;
        return -1;
    }

    UnkPointNode_28f954* NewNode(const CVector2D& p) {
        UnkPointNode_28f954* n = static_cast<UnkPointNode_28f954*>(allocator->vf_0x40(0x10, 8, 0x13));
        if (n) {
            n->p.x = p.x;
            n->p.y = p.y;
            n->next = nullptr;
        }
        return n;
    }

    void PushBack(const CVector2D& p) {
        UnkPointNode_28f954* n = NewNode(p);
        if (!n)
            return;
        if (count == 0) {
            count = 1;
            tail = n;
            head = n;
        } else {
            tail->next = n;
            ++count;
            tail = n;
            n->next = nullptr;
        }
    }

    void PushFront(const CVector2D& p) {
        UnkPointNode_28f954* n = NewNode(p);
        if (!n)
            return;
        if (count == 0) {
            count = 1;
            tail = n;
            head = n;
        } else {
            n->next = head;
            ++count;
            head = n;
        }
    }

    void InsertBefore(UnkPointNode_28f954* at, const CVector2D& p) {
        if (!head)
            return;
        if (at == head) {
            PushFront(p);
            return;
        }
        UnkPointNode_28f954* prev = head;
        for (UnkPointNode_28f954* n = head->next; n != at; n = n->next) {
            if (!n)
                return;
            prev = n;
        }
        if (!prev) {
            PushFront(p);
            return;
        }
        UnkPointNode_28f954* n = NewNode(p);
        if (count == 0) {
            if (!n)
                return;
            count = 1;
            tail = n;
            head = n;
            return;
        }
        if (!n)
            return;
        UnkPointNode_28f954* next = prev->next;
        prev->next = n;
        n->next = next;
        if (!next)
            tail = n;
        ++count;
    }

    // Keeps the points ordered by falling distance from `end`.
    void InsertByDistance(const CVector2D& p, const CVector2D& end) {
        if (head) {
            f32 d = DistSq(end, p);
            for (UnkPointNode_28f954* n = head; n; n = n->next) {
                if (d > DistSq(end, n->p)) {
                    InsertBefore(n, p);
                    return;
                }
            }
        }
        PushBack(p);
    }
};

// Whether p lies inside the segment (not on its ends); a new such point is recorded.
static inline bool AddSplit(const UnkSegment2D& seg, const CVector2D& end, const CVector2D& p, UnkPointList_28f954& list, f32 eps,
                            UnkNavPointArray_28f954* points) {
    if (list.Contains(p, eps))
        return false;
    if (Near1(seg.start, p) || Near1(seg.end, p))
        return false;
    if (!fn_7100125c64(&seg, &p, false, 1.0f))
        return false;
    if (list.Find(p, eps) < 0) {
        if (points)
            points->PushBack(p);
        list.InsertByDistance(p, end);
    }
    return true;
}

// What a pair test fills on a hit.
struct UnkNavContact_28f954 {
    CVector3D point;
    CVector3D normal_a;
    CVector3D normal_b;
    u32 field_0x24;
    u32 flags;
};

s32 fn_710028f954(UnkNavMeshGenerator_28f954* self, UnkNavSource_28f954* source, UnkNavEdgeSet_28f954* edges,
                  UnkNavPolygonList_28f954* out, UnkNavPointArray_28f954* points) {
    UnkNavShape_28f954* shape = source->shape;
    if (!shape || shape->vf_0x18() != 0x10)
        return 0;

    s32 added = 0;
    const f32 eps = g_unk_7101647774;
    const f32 cell = g_unk_7101648c88;
    const f32 half = cell * 0.5f;
    for (UnkNavPolygon_28f954 *poly = shape->polygons.begin(), *end = shape->polygons.end(); poly != end; ++poly) {
        UnkNavPolygon_28f954 outline;
        if (poly->dirty)
            fn_7100119b88(reinterpret_cast<UnkPolygon2D*>(poly));

        for (u64 i = 0; i < poly->EdgeCount(); ++i) {
            const UnkNavVertex_28f954* a = &poly->vertices.data[i];
            const UnkNavVertex_28f954* b =
                i == (u32)(poly->vertices.count - 1) ? &poly->vertices.data[0] : &poly->vertices.data[i + 1];
            UnkSegment2D seg(a->xy, b->xy);
            const CVector2D seg_end = seg.end;
            UnkPointList_28f954 list;

            for (UnkNavEdge_28f954* e = edges->head; e; e = e->next) {
                UnkSegment2D other(e->a, e->b);
                // A crossing is looked for only when neither end of the source edge splits this one.
                bool start_off = !poly->BoundsContain(other.start) || !AddSplit(seg, seg_end, other.start, list, eps, points);
                if (AddSplit(seg, seg_end, other.end, list, eps, points) || !start_off)
                    continue;

                UnkNavContact_28f954 hit = {};
                if (!fn_710012be70(&seg, &other, reinterpret_cast<UnkContact_710012be70*>(&hit), false, 0.0f))
                    continue;
                CVector2D q;
                fn_710007b430(&q, &hit.point);
                CVector2D snapped;
                snapped.x = Snap(q.x, cell, half);
                snapped.y = Snap(q.y, cell, half);
                if (list.Find(snapped, eps) >= 0)
                    continue;
                if (points)
                    points->PushBack(snapped);
                list.InsertByDistance(snapped, seg_end);
            }

            if (list.Find(seg_end, eps) < 0)
                list.PushBack(seg_end);

            for (UnkPointNode_28f954* n = list.head; n; n = n->next) {
                u32 count = outline.vertices.count;
                if (poly->IsLastEdge(i) && count != 0 && !n->next && Near1(n->p, outline.vertices.data[0].xy))
                    continue;
                if (count >= 2) {
                    const CVector2D* last = static_cast<const CVector2D*>(fn_7100118950(&outline.vertices, count - 1));
                    const CVector2D* prev = static_cast<const CVector2D*>(fn_7100118950(&outline.vertices, outline.vertices.count - 2));
                    if (Near1(n->p, *last) || Near1(n->p, *prev))
                        continue;
                } else if (count != 0) {
                    const CVector2D* last = static_cast<const CVector2D*>(fn_7100118950(&outline.vertices, count - 1));
                    if (Near1(n->p, *last))
                        continue;
                }
                CVector3D v;
                v.x = n->p.x;
                v.y = n->p.y;
                v.z = 0.0f;
                fn_7100118b94(&outline, reinterpret_cast<UnkVec3*>(&v));
            }
        }

        fn_7100119f98(reinterpret_cast<UnkTileOutline*>(&outline), true);
        if (outline.EdgeCount() > 2) {
            PushBackCopy(out, outline);
            out->changed = true;
            s32 gained = outline.EdgeCount() - poly->EdgeCount();
            added += gained > 0 ? gained : 0;
        }
    }
    return added;
}
