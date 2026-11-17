// Unit 365 (tools/tu_map.py, hint navmesh::CNavMeshGenerator): chains a list of loose segments into closed
// polygons: from each remaining segment, follows the best-turning segment that starts where the last one
// ends until the chain returns to its first point, then adds the polygon. Names and layouts are placeholders.
#include <math.h>
#include <new>

#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkTU108.h"
#include "game/UnkXformNode.h"
#include "game/malloc.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkVec4fE;
bool fn_7100125b70(const UnkVec4fE* a, const UnkVec4fE* b);

// A copy of a segment with its cached length cleared.
static inline void CopySegment(UnkSegment2D& dst, const UnkSegment2D& src) {
    dst.start.x = src.start.x;
    dst.start.y = src.start.y;
    dst.end.x = src.end.x;
    dst.end.y = src.end.y;
    dst.field_0x10 = -1.0f;
}

static inline void ClearSegment(UnkSegment2D& segment) {
    segment.start = CVector2D{0.0f, 0.0f};
    segment.end = CVector2D{0.0f, 0.0f};
    segment.field_0x10 = -1.0f;
}

// A singly linked list of segments, its nodes from an allocator.
struct UnkSegmentList_l293e30 {
    struct Node {
        UnkSegment2D value;
        Node* next;
    };
    u64 field_0x0;
    u32 count;
    Node* head;
    Node* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    UnkSegmentList_l293e30(const UnkSegmentList_l293e30& other)
        : count(0), head(nullptr), tail(nullptr), allocator(other.allocator), flags(other.flags) {
        for (Node* n = other.head; n; n = n->next)
            PushBack(n->value);
    }

    void PushBack(const UnkSegment2D& value) {
        Node* n = static_cast<Node*>(allocator->vf_0x40(sizeof(Node), 8, flags));
        if (!n)
            return;
        CopySegment(n->value, value);
        n->next = nullptr;
        if (count == 0) {
            count = 1;
            head = tail = n;
        } else {
            tail->next = n;
            ++count;
            tail = n;
            n->next = nullptr;
        }
    }

    void Remove(Node* node) {
        Node* prev = nullptr;
        for (Node* it = head; it; prev = it, it = it->next) {
            if (it != node)
                continue;
            if (prev)
                prev->next = it->next;
            else
                head = it->next;
            if (--count == 0)
                head = tail = nullptr;
            allocator->vf_0x48(node);
            return;
        }
    }
};

// A polygon: its vertex array, then the cached length, flags, bounds and winding state.
struct UnkNavPolygon_l293e30 {
    base::global::CRntVector<UnkVec3f> vertices;
    f32 field_0x28;
    bool closed;  // 0x2c
    bool dirty;   // 0x2d
    u8 pad_0x2e[2];
    u64 bounds_min;  // 0x30, two floats
    u64 bounds_max;  // 0x38, two floats
    u32 state;       // 0x40
    u32 pad_0x44;

    UnkNavPolygon_l293e30()
        : vertices(7), field_0x28(0.0f), closed(false), dirty(true), bounds_min(0x7f7fffff7f7fffffull),
          bounds_max(0xff7fffffff7fffffull), state(0) {}

    UnkNavPolygon_l293e30(const UnkNavPolygon_l293e30& other) {
        fn_710011d8b4(reinterpret_cast<UnkPart0*>(&vertices), reinterpret_cast<const UnkPart0*>(&other.vertices));
        field_0x28 = other.field_0x28;
        closed = other.closed;
        dirty = true;
        bounds_min = 0x7f7fffff7f7fffffull;
        bounds_max = 0xff7fffffff7fffffull;
        state = other.state;
    }

    ~UnkNavPolygon_l293e30() { vertices.Free(); }
};
static_assert(sizeof(UnkNavPolygon_l293e30) == 0x48, "UnkNavPolygon_l293e30 layout");

// The generated polygons: an array in base::global::CRntVector's layout, then a changed flag.
struct UnkNavPolygonSet_l293e30 {
    UnkNavPolygon_l293e30* data;
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

static inline bool IsNear(f32 a, f32 b) {
    return a + 0.001f >= b && a - 0.001f <= b;
}

static inline bool IsNear(const CVector2D& a, const CVector2D& b) {
    return IsNear(a.x, b.x) && IsNear(a.y, b.y);
}

static inline CVector2D Direction(const UnkSegment2D& segment) {
    CVector2D d;
    d.x = segment.end.x - segment.start.x;
    d.y = segment.end.y - segment.start.y;
    f32 length = sqrtf(d.x * d.x + d.y * d.y);
    if (length != 0.0f) {
        f32 inv = 1.0f / length;
        d.x *= inv;
        d.y *= inv;
    }
    return d;
}

void fn_7100293e30(void* self, const UnkSegmentList_l293e30* input, UnkNavPolygonSet_l293e30* polygons) {
    UnkSegmentList_l293e30 segments(*input);
    while (segments.head) {
        UnkSegment2D first;
        CopySegment(first, segments.head->value);
        segments.Remove(segments.head);
        UnkNavPolygon_l293e30 polygon;
        fn_7100119f98(reinterpret_cast<UnkTileOutline*>(&polygon), true);
        UnkSegment2D current;
        CopySegment(current, first);
        for (;;) {
            CVector3D point = {current.start.x, current.start.y, 0.0f};
            fn_7100118b94(&polygon, reinterpret_cast<UnkVec3*>(&point));
            if (IsNear(current.end, first.start)) {
                // Append a copy of the polygon, growing the array to the next multiple of its step.
                u32 n = polygons->count;
                u32 capacity = polygons->capacity;
                if (capacity <= n && polygons->step != 0)
                    capacity = (n / polygons->step + 1) * polygons->step;
                if ((s32)n >= (s32)capacity)
                    for (;;) {
                    }
                polygons->Reserve(capacity);
                new (&polygons->data[polygons->count]) UnkNavPolygon_l293e30(polygon);
                ++polygons->count;
                polygons->changed = true;
                break;
            }
            CVector2D dir = Direction(current);
            UnkSegment2D best;
            ClearSegment(best);
            UnkVec2Result normal = fn_7100125e78(&current);
            UnkSegment2D candidate;
            ClearSegment(candidate);
            f32 best_score = -3.40282347e+38f;
            for (UnkSegmentList_l293e30::Node* node = segments.head; node; node = node->next) {
                if (!IsNear(current.end.x, node->value.start.x) || !IsNear(current.end.y, node->value.start.y))
                    continue;
                CVector2D next_dir = Direction(node->value);
                f32 cos = dir.x * -next_dir.x + dir.y * -next_dir.y;
                f32 side = next_dir.x * normal.x + next_dir.y * normal.y;
                bool left = !(side + 0.001f >= 0.0f && side - 0.001f <= 0.0f) && side > 0.0f;
                f32 score = (left ? cos + 3.0f : 1.0f - cos) * 0.25f;
                if (score > best_score) {
                    fn_7100125b44(&candidate, &node->value);
                    best_score = score;
                }
            }
            if (best_score == -3.40282347e+38f)
                break;
            fn_7100125b44(&best, &candidate);
            for (UnkSegmentList_l293e30::Node* node = segments.head; node; node = node->next) {
                if (fn_7100125b70(reinterpret_cast<const UnkVec4fE*>(&node->value),
                                  reinterpret_cast<const UnkVec4fE*>(&candidate))) {
                    segments.Remove(node);
                    break;
                }
            }
            fn_7100125b44(&current, &best);
        }
    }
}
