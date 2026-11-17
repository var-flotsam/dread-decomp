// Unit 365 ("navmesh::CNavMeshGenerator"): for each link of a list, finds the polygon edges that end at
// its two points and files a record of them in one of two lists, by whether the edges run on in line.
// Names and layouts are placeholders.
#include <math.h>
#include <new>
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkEdgeCursor_b0228;
UnkSegment2D fn_7100289128(UnkEdgeCursor_b0228* self);

struct UnkPolygon_l290b20 {
    CVector3D* points;  // 0x0
    u32 count;          // 0x8
    u8 pad_0xc[0x20];
    bool closed;  // 0x2c
    u8 pad_0x2d[0x1b];
};
static_assert(sizeof(UnkPolygon_l290b20) == 0x48, "UnkPolygon_l290b20 layout");

struct UnkPolygonArray_l290b20 {
    UnkPolygon_l290b20* data;  // 0x0
    u32 count;                 // 0x8
};

struct UnkLink_l290b20 {
    u8 pad_0x0[0xc];
    CVector2D a;  // 0xc
    u8 pad_0x14[0x10];
    CVector2D b;  // 0x24
    u8 pad_0x2c[4];
    UnkLink_l290b20* next;  // 0x30
};

struct UnkLinkList_l290b20 {
    u8 pad_0x0[0x10];
    UnkLink_l290b20* head;  // 0x10
};

// The edges found for a link: polygon and edge index at each end, or the link's own point when no
// edge ends there.
struct UnkEdgePair_l290b20 {
    UnkPolygon_l290b20* first;  // 0x0
    u32 first_index;            // 0x8
    UnkPolygon_l290b20* second;  // 0x10
    u32 second_index;            // 0x18
    CVector2D first_fallback;    // 0x1c
    CVector2D second_fallback;   // 0x24
};

struct UnkEdgeRecord_l290b20 {
    UnkEdgePair_l290b20 pair;      // 0x0
    UnkEdgeRecord_l290b20* next;  // 0x30
};

struct UnkEdgeRecordList_l290b20 {
    u8 pad_0x0[8];
    u32 count;                      // 0x8
    UnkEdgeRecord_l290b20* head;    // 0x10
    UnkEdgeRecord_l290b20* tail;    // 0x18
    base::UnkAllocator* allocator;  // 0x20
    u32 flags;                      // 0x28

    void PushBack(const UnkEdgePair_l290b20& pair) {
        UnkEdgeRecord_l290b20* record =
            static_cast<UnkEdgeRecord_l290b20*>(allocator->vf_0x40(sizeof(UnkEdgeRecord_l290b20), 8, flags));
        if (!record)
            return;
        record->pair = pair;
        record->next = nullptr;
        if (count == 0) {
            head = record;
            tail = record;
        } else {
            tail->next = record;
            record->next = nullptr;
            tail = record;
        }
        count = count + 1;
    }
};

static inline u32 EdgeCount(const UnkPolygon_l290b20* poly) {
    return poly->count - (!poly->closed && poly->count != 0);
}

static inline void EdgeOf(UnkSegment2D* seg, UnkPolygon_l290b20* poly, u32 index) {
    CVector3D* start = &poly->points[index];
    CVector3D* next = poly->count - 1 == index ? poly->points : &poly->points[index + 1];
    new (seg) UnkSegment2D(*reinterpret_cast<CVector2D*>(start), *reinterpret_cast<CVector2D*>(next));
}

// The edge after `index`; past the end of an open polygon, a point at its last vertex.
static inline void NextEdgeOf(UnkSegment2D* seg, UnkPolygon_l290b20* poly, u32 index) {
    if (!poly->closed && index >= EdgeCount(poly) - 1) {
        UnkSegment2D last;
        EdgeOf(&last, poly, index);
        new (seg) UnkSegment2D(last.end, last.end);
    } else {
        EdgeOf(seg, poly, index == EdgeCount(poly) - 1 ? 0 : index + 1);
    }
}

static inline bool IsNear(f32 v, f32 target) { return v - 1.0f <= target && v + 1.0f >= target; }

static inline bool IsNear(const CVector2D& p, const CVector2D& target) {
    return IsNear(p.x, target.x) && IsNear(p.y, target.y);
}

static inline bool IsOne(f32 v) { return v + 0.001f >= 1.0f && v - 0.001f <= 1.0f; }

static inline void Normalize(f32& x, f32& y) {
    f32 len = sqrtf(x * x + y * y);
    if (len != 0.0f) {
        f32 inv = 1.0f / len;
        x *= inv;
        y *= inv;
    }
}

void fn_7100290b20(void* self, UnkPolygonArray_l290b20* polys, UnkLinkList_l290b20* links,
                   UnkEdgeRecordList_l290b20* bent, UnkEdgeRecordList_l290b20* straight, bool keep_unmatched) {
    for (UnkLink_l290b20* link = links->head; link; link = link->next) {
        UnkEdgePair_l290b20 pair;
        pair.first = nullptr;
        pair.first_index = 0;
        pair.second = nullptr;
        pair.second_index = 0;
        pair.first_fallback.x = 0.0f;
        pair.first_fallback.y = 0.0f;
        pair.second_fallback.x = 0.0f;
        pair.second_fallback.y = 0.0f;

        UnkPolygon_l290b20* poly_a = nullptr;
        u32 index_a = 0;
        UnkPolygon_l290b20* poly_b = nullptr;
        u32 index_b = 0;
        UnkPolygon_l290b20* end = polys->data + polys->count;
        for (UnkPolygon_l290b20* poly = polys->data; poly != end; ++poly) {
            fn_7100083990();
            for (u64 i = 0; i < EdgeCount(poly); ++i) {
                UnkSegment2D seg;
                CVector3D* start = &poly->points[i];
                CVector3D* next = poly->count - 1 == i ? poly->points : &poly->points[i + 1];
                new (&seg) UnkSegment2D(*reinterpret_cast<CVector2D*>(start), *reinterpret_cast<CVector2D*>(next));
                if (IsNear(seg.end, link->a)) {
                    pair.first = poly;
                    pair.first_index = i;
                    poly_a = poly;
                    index_a = i;
                } else if (IsNear(seg.end, link->b)) {
                    pair.second = poly;
                    pair.second_index = i;
                    poly_b = poly;
                    index_b = i;
                }
            }
        }

        if (poly_a && poly_b) {
            UnkSegment2D edge_a;
            EdgeOf(&edge_a, poly_a, index_a);
            UnkSegment2D edge_b;
            EdgeOf(&edge_b, poly_b, index_b);
            UnkSegment2D next_a;
            NextEdgeOf(&next_a, poly_a, index_a);
            UnkSegment2D next_b;
            NextEdgeOf(&next_b, poly_b, index_b);

            f32 across_x = edge_b.end.x - edge_a.end.x;
            f32 across_y = edge_b.end.y - edge_a.end.y;
            Normalize(across_x, across_y);
            f32 back_a_x = edge_a.start.x - edge_a.end.x;
            f32 back_a_y = edge_a.start.y - edge_a.end.y;
            Normalize(back_a_x, back_a_y);
            f32 back_b_x = edge_b.start.x - edge_b.end.x;
            f32 back_b_y = edge_b.start.y - edge_b.end.y;
            Normalize(back_b_x, back_b_y);
            f32 on_a_x = next_a.end.x - next_a.start.x;
            f32 on_a_y = next_a.end.y - next_a.start.y;
            Normalize(on_a_x, on_a_y);
            f32 on_b_x = next_b.end.x - next_b.start.x;
            f32 on_b_y = next_b.end.y - next_b.start.y;
            Normalize(on_b_x, on_b_y);

            UnkSegment2D link_seg(link->a, link->b);
            fn_7100289128(reinterpret_cast<UnkEdgeCursor_b0228*>(&pair));

            bool in_line_a = IsOne(across_x * back_a_x + across_y * back_a_y) ||
                             IsOne(across_x * on_a_x + across_y * on_a_y);
            bool in_line_b = IsOne(back_b_x * -across_x - back_b_y * across_y) ||
                             IsOne(on_b_y * -across_y - on_b_x * across_x);
            if (in_line_a && in_line_b)
                straight->PushBack(pair);
            else
                bent->PushBack(pair);
        } else if (keep_unmatched) {
            if (!poly_a)
                pair.first_fallback = link->a;
            if (!poly_b)
                pair.second_fallback = link->b;
            bent->PushBack(pair);
        }
    }
}
