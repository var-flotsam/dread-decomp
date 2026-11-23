// Unit 365 (navmesh::CNavMeshGenerator's): rebuilds every polygon of a list but one as an outline (the
// end point of each of its segments, flattened), finishes each outline and appends it to the result,
// marking the result changed. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkTU108.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;

struct UnkVec3 {
    f32 x;
    f32 y;
    f32 z;
};

// base::global::CRntVector's layout with plain members, so that the copy below can build it.
struct UnkNavVertices_l28e71c {
    UnkVec3f* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

// A 0x48-byte polygon of unit 108: its vertices, then a cached length, flags, bounds and winding state.
struct UnkNavPolygon_l28e71c {
    UnkNavVertices_l28e71c vertices;
    f32 field_0x28;
    bool closed;  // 0x2c
    bool dirty;   // 0x2d
    CVector2D bounds_min;  // 0x30
    CVector2D bounds_max;  // 0x38
    u32 state;  // 0x40

    UnkNavPolygon_l28e71c() {
        vertices.data = nullptr;
        vertices.count = 0;
        vertices.capacity = 0;
        vertices.step = 10;
        vertices.field_0x14 = true;
        vertices.flags = 7;
        vertices.allocator = GetDefaultAllocator();
        field_0x28 = 0.0f;
        closed = false;
        dirty = true;
        ResetBounds();
        state = 0;
    }

    UnkNavPolygon_l28e71c(const UnkNavPolygon_l28e71c& other) {
        fn_710011d8b4(reinterpret_cast<UnkPart0*>(&vertices), reinterpret_cast<const UnkPart0*>(&other.vertices));
        field_0x28 = other.field_0x28;
        closed = other.closed;
        dirty = true;
        ResetBounds();
        state = other.state;
    }

    ~UnkNavPolygon_l28e71c() {
        if (vertices.data) {
            vertices.count = 0;
            vertices.allocator->vf_0x48(vertices.data);
            vertices.capacity = 0;
        }
    }

    void ResetBounds() {
        bounds_min.x = 3.40282347e+38f;
        bounds_min.y = 3.40282347e+38f;
        bounds_max.x = -3.40282347e+38f;
        bounds_max.y = -3.40282347e+38f;
    }

    // An open polygon has one segment fewer than it has vertices.
    u32 SegmentCount() const { return vertices.count - (!closed && vertices.count != 0); }

    // The segment from vertex i to the next one, the last wrapping round to the first.
    UnkSegment2D Segment(u32 i) const {
        const UnkVec3f* from = &vertices.data[i];
        const UnkVec3f* to = i == vertices.count - 1 ? &vertices.data[0] : &vertices.data[i + 1];
        return UnkSegment2D(reinterpret_cast<const CVector2D&>(*from), reinterpret_cast<const CVector2D&>(*to));
    }
};
static_assert(sizeof(UnkNavPolygon_l28e71c) == 0x48, "layout");

// base::global::CRntVector's layout over the polygons.
struct UnkNavPolygonArray_l28e71c {
    UnkNavPolygon_l28e71c* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    // Out of line: grows the array to at least n elements.
    void Reserve(u32 n) asm("fn_710011f1ec");

    void PushBack(const UnkNavPolygon_l28e71c& value) {
        u32 n = count;
        u32 cap = capacity;
        u32 new_capacity = cap;
        if (cap <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        // A capacity that cannot grow hangs, as in the original.
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        Reserve(new_capacity);
        new (&data[count]) UnkNavPolygon_l28e71c(value);
        ++count;
    }

    const UnkNavPolygon_l28e71c* begin() const { return data; }
    const UnkNavPolygon_l28e71c* end() const { return data + count; }
};

struct UnkNavOutlines_l28e71c {
    UnkNavPolygonArray_l28e71c polygons;
    u8 pad_0x28[0x38 - 0x28];
    bool changed;
};

void fn_710028e71c(void* self, const UnkNavPolygonArray_l28e71c* polygons, const UnkNavPolygon_l28e71c* skip,
                   UnkNavOutlines_l28e71c* out) {
    for (const UnkNavPolygon_l28e71c& polygon : *polygons) {
        if (&polygon == skip)
            continue;
        UnkNavPolygon_l28e71c outline;
        for (u32 i = 0; i < polygon.SegmentCount(); ++i) {
            UnkSegment2D segment = polygon.Segment(i);
            UnkVec3 point = {segment.end.x, segment.end.y, 0.0f};
            fn_7100118b94(&outline, &point);
        }
        fn_7100119f98(reinterpret_cast<UnkTileOutline*>(&outline), true);
        out->polygons.PushBack(outline);
        out->changed = true;
    }
}
