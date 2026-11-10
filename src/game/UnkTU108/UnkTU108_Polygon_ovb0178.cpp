// Unit 108 (tools/tu_map.py, 0x7100110c54-0x71001239b0; no name hint): 2D polygon helpers and the
// arrays they live in. The polygons are 0x48-byte records (a vertex array of 12-byte points, a
// closed flag at 0x2c, a dirty flag at 0x2d, bounds at 0x30 and the winding state at 0x40); the
// arrays are base::UnkVector's layout. Names and layouts are placeholders.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include <new>

#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;

// ---- An array of 8-byte pairs and one of 8-byte links ----

// An 8-byte element of two words, copied member by member.
struct UnkPathPair {
    u32 value[2];

    // Each word is copied bitwise.
    void CopyFrom(const UnkPathPair& other) {
        __builtin_memcpy(&value[0], &other.value[0], sizeof(u32));
        __builtin_memcpy(&value[1], &other.value[1], sizeof(u32));
    }
    UnkPathPair(const UnkPathPair& other) { CopyFrom(other); }
    UnkPathPair(UnkPathPair&& other) { CopyFrom(other); }
};

// Allocates new_capacity elements and moves the elements over, freeing the old array.
template <class T>
__attribute__((always_inline)) static inline T* MoveTo(base::UnkVector<T>* self, u32 new_capacity) {
    T* p = static_cast<T*>(self->allocator->vf_0x40((u64)new_capacity * sizeof(T), 8, self->flags));
    if (self->data) {
        T* src = self->data;
        T* dst = p;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) T(static_cast<T&&>(*src));
            src->~T();
        }
        self->allocator->vf_0x48(self->data);
    }
    return p;
}

// Copy constructor: the source settings and capacity, then each element appended.
__attribute__((flatten)) void fn_7100121d90(base::UnkVector<UnkPathPair>* self,
                                            const base::UnkVector<UnkPathPair>* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    for (const UnkPathPair *it = other->data, *end = it + other->count; it != end; ++it)
        self->PushBackCopy(*it);
}

// base::UnkVector::Insert with the new element's slot taken in each branch.
template <class T>
__attribute__((always_inline)) static inline void InsertAt(base::UnkVector<T>* self, T* value, u32 index) {
    u32 cap = self->capacity;
    u32 n = self->count;
    if (n == index) {
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = (index / self->step + 1) * self->step;
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        T* p;
        if (cap < new_capacity) {
            p = MoveTo(self, new_capacity);
            index = self->count;
            self->data = p;
            self->capacity = new_capacity;
        } else {
            p = self->data;
        }
        new (&p[index]) T(static_cast<T&&>(*value));
    } else {
        if (cap <= n && self->step != 0) {
            u32 grown = (n / self->step + 1) * self->step;
            if (cap < grown) {
                T* p = MoveTo(self, grown);
                n = self->count;
                self->data = p;
                self->capacity = grown;
            }
        }
        for (u32 i = n - 1; i >= index; --i) {
            T* q = &self->data[i];
            new (q + 1) T(static_cast<T&&>(*q));
            self->data[i].~T();
            if (i == 0)
                break;
        }
        new (&self->data[index]) T(static_cast<T&&>(*value));
    }
    ++self->count;
}

void fn_7100122160(base::UnkVector<UnkPathPair>* self, UnkPathPair* value, u32 index) {
    InsertAt(self, value, index);
}

struct UnkPathLink {
    u32 field_0x0;
    u32 field_0x4;

    // Zeroed and copied word by word, untyped: the original reloads the array after each store.
    UnkPathLink() {
        __builtin_memset(&field_0x0, 0, sizeof(u32));
        __builtin_memset(&field_0x4, 0, sizeof(u32));
    }
    UnkPathLink(UnkPathLink&& other) {
        __builtin_memcpy(&field_0x0, &other.field_0x0, sizeof(u32));
        __builtin_memcpy(&field_0x4, &other.field_0x4, sizeof(u32));
    }
};

__attribute__((flatten)) UnkPathLink* fn_7100122590(base::UnkVector<UnkPathLink>* self, u32 index) {
    return &self->GrowAt(index);
}

// ---- The polygon array ----

struct UnkPart0;
void fn_710011d8b4(UnkPart0* self, const UnkPart0* other);

// base::UnkVector's layout with plain members.
struct UnkVertexArray {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

// A polygon: its vertex array, then the cached length, flags, bounds and winding state.
struct UnkPolygon48 {
    UnkVertexArray vertices;
    f32 field_0x28;
    bool closed;  // 0x2c
    bool dirty;   // 0x2d
    u8 pad_0x2e[2];
    u64 bounds_min;  // 0x30, two floats
    u64 bounds_max;  // 0x38, two floats
    u32 state;       // 0x40
    u32 pad_0x44;

    UnkPolygon48() {
        new (&vertices) base::UnkVector<UnkVec3f>(7);
        field_0x28 = 0.0f;
        closed = false;
        dirty = true;
        bounds_min = 0x7f7fffff7f7fffffull;
        bounds_max = 0xff7fffffff7fffffull;
        state = 0;
    }

    UnkPolygon48(const UnkPolygon48& other) {
        fn_710011d8b4(reinterpret_cast<UnkPart0*>(&vertices), reinterpret_cast<const UnkPart0*>(&other.vertices));
        field_0x28 = other.field_0x28;
        closed = other.closed;
        dirty = true;
        bounds_min = 0x7f7fffff7f7fffffull;
        bounds_max = 0xff7fffffff7fffffull;
        state = other.state;
    }

    UnkPolygon48(UnkPolygon48&& other) {
        dirty = true;
        __builtin_memcpy(&vertices, &other.vertices, 16);
        __builtin_memset(&other.vertices, 0, 16);
        vertices.step = other.vertices.step;
        vertices.field_0x14 = other.vertices.field_0x14;
        vertices.flags = other.vertices.flags;
        vertices.allocator = other.vertices.allocator;
        field_0x28 = other.field_0x28;
        closed = other.closed;
        bounds_min = 0x7f7fffff7f7fffffull;
        bounds_max = 0xff7fffffff7fffffull;
        state = other.state;
    }

    ~UnkPolygon48() { reinterpret_cast<base::UnkVector<UnkVec3f>*>(&vertices)->Free(); }
};

struct UnkPolygonArray {
    UnkPolygon48* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    // Out of line: grows the array to at least n elements.
    void Reserve(u32 n) asm("fn_710011f1ec");

    void PushBackCopy(const UnkPolygon48& value) {
        u32 n = count;
        u32 cap = capacity;
        u32 new_capacity = cap;
        if (cap <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        Reserve(new_capacity);
        new (&data[count]) UnkPolygon48(value);
        ++count;
    }
};

void fn_710011f0d8(UnkPolygonArray* self, const UnkPolygonArray* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    for (const UnkPolygon48 *it = other->data, *end = it + other->count; it != end; ++it)
        self->PushBackCopy(*it);
}

UnkPolygonArray* fn_710011f340(UnkPolygonArray* self, const UnkPolygonArray* other) {
    if (self != other) {
        for (u32 i = 0; i < self->count; ++i)
            self->data[i].~UnkPolygon48();
        self->count = 0;
        self->Reserve(other->capacity);
        for (const UnkPolygon48 *it = other->data, *end = it + other->count; it != end; ++it)
            self->PushBackCopy(*it);
    }
    return self;
}

struct UnkEntryVector108_b67a1 : UnkPolygonArray {};

// Destroys the polygon at index and moves the later ones down.
void fn_710011f95c(UnkEntryVector108_b67a1* v, u32 index) {
    if (index < v->count) {
        u64 i = index;
        v->data[i].~UnkPolygon48();
        for (; i < (u32)(v->count - 1); ++i) {
            UnkPolygon48* cur = &v->data[i];
            new (cur) UnkPolygon48(static_cast<UnkPolygon48&&>(cur[1]));
            (&v->data[i])[1].~UnkPolygon48();
        }
        v->count = v->count - 1;
    }
}

// ---- A polygon's vertices ----

// A 12-byte vertex, zeroed on construction and moved a word at a time (untyped).
struct UnkVertex12 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;

    UnkVertex12() {
        __builtin_memset(&field_0x0, 0, sizeof(u32));
        __builtin_memset(&field_0x4, 0, sizeof(u32));
        __builtin_memset(&field_0x8, 0, sizeof(u32));
    }
    UnkVertex12(UnkVertex12&& other) {
        __builtin_memcpy(&field_0x0, &other.field_0x0, sizeof(u32));
        __builtin_memcpy(&field_0x4, &other.field_0x4, sizeof(u32));
        __builtin_memcpy(&field_0x8, &other.field_0x8, sizeof(u32));
    }
};

struct UnkPointSet_b61 : base::UnkVector<UnkVertex12> {};

// The vertex at index, growing the array to it first.
__attribute__((noinline)) void* fn_7100118950(UnkPointSet_b61* self, u32 index) {
    return &self->GrowAt(index);
}

struct UnkPolyPoint {
    f32 x;
    f32 y;
    f32 field_0x8;
};

struct UnkPolygonBase {
    void* vertices;
    u32 count;
    u8 unk_0xc[0x20];
    u8 closed;  // 0x2c
    u8 pad_0x2d[0x13];
    u32 state;  // 0x40

    // The number of edges: one per vertex when closed, one fewer otherwise.
    __attribute__((always_inline)) u32 EdgeCount() const { return count - (!closed & (count != 0)); }

    // The edge from vertex i to the next one (the first after the last).
    __attribute__((always_inline)) UnkSegment2D Edge(u32 i) const {
        UnkPolyPoint* v = static_cast<UnkPolyPoint*>(vertices);
        const UnkPolyPoint* cur = &v[i];
        const UnkPolyPoint* next = count - 1 == i ? v : &v[i + 1];
        return UnkSegment2D(*reinterpret_cast<const CVector2D*>(cur), *reinterpret_cast<const CVector2D*>(next));
    }
};

struct UnkPolygon_b61 : UnkPolygonBase {};

// Reverses the x and y of the vertices (z stays).
void fn_7100118c78(UnkPolygon_b61* self) {
    UnkPointSet_b61* points = reinterpret_cast<UnkPointSet_b61*>(self);
    u32 edges = self->EdgeCount();
    if (edges == 0)
        return;
    u32 last = edges - 1;
    f32 half = (f32)last * 0.5f;
    if (!(half > 0.0f))
        return;
    for (u32 i = 0; half > (f32)i; ++i, --last) {
        const u32* a = static_cast<const u32*>(fn_7100118950(points, i));
        u32 x = a[0];
        u32 y = a[1];
        const u32* b = static_cast<const u32*>(fn_7100118950(points, last));
        u32* dst = static_cast<u32*>(fn_7100118950(points, i));
        dst[0] = b[0];
        dst[1] = b[1];
        u32* end = static_cast<u32*>(fn_7100118950(points, last));
        end[0] = x;
        end[1] = y;
    }
}

// ---- Winding and edge queries ----

// Sets a polygon's winding direction (state 1 or 2), reversing the point list when the signed area
// disagrees with the requested direction.
void fn_7100119d3c(UnkPolygon_b61* self, bool flag) {
    u32 state = self->state;
    if (flag) {
        if (state == 2)
            return;
    } else if (state == 1) {
        return;
    }
    if (self->closed) {
        f32 area = 0.0f;
        UnkPolyPoint* points = static_cast<UnkPolyPoint*>(self->vertices);
        for (u64 i = 0; i != self->count; ++i) {
            u64 next = i < self->count - 1 ? i + 1 : 0;
            UnkPolyPoint a = points[i];
            UnkPolyPoint b = points[next];
            area += (b.x - a.x) * 0.5f * (a.y + b.y);
        }
        if ((area >= 0.0f) ^ flag)
            fn_7100118c78(self);
    }
    self->state = flag ? 2 : 1;
}

struct UnkTarget108 : UnkPolygonBase {};

// The signed area of the polygon (the shoelace sum over its points).
static inline __attribute__((always_inline)) f32 SignedArea(const UnkPolygonBase* p) {
    f32 area = 0.0f;
    UnkPolyPoint* points = static_cast<UnkPolyPoint*>(p->vertices);
    for (u64 i = 0; i != p->count; ++i) {
        u64 next = i < p->count - 1 ? i + 1 : 0;
        UnkPolyPoint a = points[i];
        UnkPolyPoint b = points[next];
        area += (b.x - a.x) * 0.5f * (a.y + b.y);
    }
    return area;
}

// Turns the pending winding states 3 and 4 into 2 and 1, reversing the points when needed.
void fn_7100119c1c(UnkTarget108* target) {
    if (!target->closed)
        return;
    u32 state = target->state;
    if (state == 4) {
        if (!(SignedArea(target) >= 0.0f))
            fn_7100118c78(reinterpret_cast<UnkPolygon_b61*>(target));
        target->state = 2;
    } else if (state == 3) {
        if (SignedArea(target) >= 0.0f)
            fn_7100118c78(reinterpret_cast<UnkPolygon_b61*>(target));
        target->state = 1;
    }
}

// An empty std::function's layout (callable storage, then the callable pointer); passed in memory.
struct alignas(16) UnkEmptyCallback {
    u8 storage[0x20];
    void* callable;
    u8 pad_0x28[8];
    UnkEmptyCallback() : callable(nullptr) {}
    UnkEmptyCallback(const UnkEmptyCallback&) {}
};
using UnkPointFilter = UnkEmptyCallback;
using UnkEdgeVisitor = UnkEmptyCallback;

struct UnkPolygon2D;
void fn_7100117ba0(const UnkPolygon2D* polygon, const CVector2D* point, s32* edge, f32* t, CVector2D* closest,
                   f32* distance, bool flag, UnkPointFilter filter, UnkEdgeVisitor visit);

struct UnkPolygon2D_l118124 : UnkPolygonBase {};

// Finds the polygon edge nearest a point and, when the closest point is at an edge end, reports the
// neighbouring edge and which end it shares.
bool fn_7100118124(const UnkPolygon2D_l118124* polygon, const CVector2D* point, s32* edge, f32* t,
                   CVector2D* closest, f32* distance, s32* out_edge, f32* out_end) {
    CVector2D found = {};
    fn_7100117ba0((const UnkPolygon2D*)polygon, point, edge, t, &found, distance, false, UnkPointFilter(),
                  UnkEdgeVisitor());
    if (closest)
        closest->x = found.x, closest->y = found.y;
    if (!out_edge || !out_end)
        return false;
    f32 lo = *t - 0.001f;
    f32 hi = *t + 0.001f;
    if (hi >= 0.0f && lo <= 0.0f) {
        s32 e = *edge;
        if (e == 0)
            e = polygon->EdgeCount();
        *out_edge = e - 1;
        const u32 one = 0x3f800000;
        __builtin_memcpy(out_end, &one, 4);
    } else {
        if (!(hi >= 1.0f && lo <= 1.0f))
            return false;
        s32 e = *edge;
        *out_edge = (polygon->EdgeCount() - 1 == e) ? 0 : e + 1;
        const u32 zero = 0;
        __builtin_memcpy(out_end, &zero, 4);
    }
    return true;
}

struct UnkPolygon2D : UnkPolygonBase {};

// The edge before or after vertex along dir: its index is stored in *a, the dot product in *b.
UnkSegment2D fn_71001190a0(UnkPolygon2D* polygon, u32 vertex, const UnkVec2Result* dir, u32* a, u32* b) {
    UnkSegment2D seg = polygon->Edge(vertex);
    f32 ex = seg.end.x - seg.start.x;
    f32 ey = seg.end.y - seg.start.y;
    f32 dot = ex * dir->x + ey * dir->y;
    __builtin_memcpy(b, &dot, 4);
    u32 next;
    if (dot > 0.0f)
        next = polygon->EdgeCount() - 1 == vertex ? 0 : vertex + 1;
    else
        next = (vertex == 0 ? polygon->EdgeCount() : vertex) - 1;
    *a = next;
    return polygon->Edge(next);
}

u32 fn_710011884c(UnkPolygon2D* polygon, u32 vertex, const UnkVec2Result* dir) {
    UnkSegment2D seg = polygon->Edge(vertex);
    f32 ex = seg.end.x - seg.start.x;
    f32 ey = seg.end.y - seg.start.y;
    f32 dot = ex * dir->x + ey * dir->y;
    if (dot > 0.0f)
        return polygon->EdgeCount() - 1 == vertex ? 0 : vertex + 1;
    return (vertex == 0 ? polygon->EdgeCount() : vertex) - 1;
}

// Whether the edge after the vertex continues along dir.
bool fn_7100118788(UnkPolygon2D* polygon, u32 vertex, const UnkVec2Result* dir) {
    UnkSegment2D seg = polygon->Edge(vertex);
    f32 ex = seg.end.x - seg.start.x;
    f32 ey = seg.end.y - seg.start.y;
    f32 dot = ex * dir->x + ey * dir->y;
    bool closed = polygon->closed;
    if (dot > 0.0f) {
        if (closed)
            return true;
        return polygon->EdgeCount() - 1 > vertex;
    }
    return vertex != 0 || closed;
}
