// Unit 365 (navmesh::CNavMeshGenerator's neighbours): for each edge of a polyline, tests the edge
// (lengthened by 10 at both ends) against a navmesh shape, snaps each end to the end of a polygon edge it
// touches, and adds the edge to a list unless an edge with the same ends is already there.
// Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkTU109.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU2517.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/CCollider.h"
#include "game/logic/collision/UnkColliderResult.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkShapeFilter_ovb0107;

extern void* g_unk_7101a210f8[];

// A collision shape (0xc0 bytes) built from a segment and its destructor.
struct UnkSegmentShape {
    u8 data[0xc0];
};

// A polyline: its points, their count and whether it closes on itself.
struct UnkNavPolyline_l29567c {
    CVector3D* points;
    u32 count;
    u8 pad_0xc[0x20];
    bool closed;  // 0x2c

    // An open polyline has one edge fewer than it has points.
    u32 EdgeCount() const { return count - (count != 0 && !closed); }
};

// The ends of the edge of a polyline from point `index` to the next one.
static inline const CVector2D& EdgeStart(const UnkNavPolyline_l29567c* line, u32 index) {
    return reinterpret_cast<const CVector2D&>(line->points[index]);
}
static inline const CVector2D& EdgeEnd(const UnkNavPolyline_l29567c* line, u32 index) {
    const CVector3D* points = line->points;
    return reinterpret_cast<const CVector2D&>(index == line->count - 1 ? points[0] : points[index + 1]);
}

struct UnkNavShape_l29567c {
    void** vtable;
    u8 pad_0x8[0xa0];
    u8 polygons[0x8];  // 0xa8: the polygons' pool

    s32 vf_0x18() { return reinterpret_cast<s32 (*)(UnkNavShape_l29567c*)>(vtable[0x18 / 8])(this); }
};

struct UnkNavArea_l29567c {
    u8 pad_0x0[0x158];
    UnkNavShape_l29567c* shape;  // 0x158
};

// One hit of the segment: the polygon edge (its polygon's handle, the edge's index in the low half) and
// the point.
struct UnkNavHit_l29567c {
    u32 handle;
    CVector3D point;
    u8 pad_0x10[0x20];
};

// The hits of a segment test, up to 128.
struct UnkNavHits_l29567c {
    void** vtable;
    u32 count;
    UnkNavHit_l29567c hits[128];
    CVector3D origin;

    explicit UnkNavHits_l29567c(const CVector2D& o) {
        vtable = g_unk_7101a210f8 + 2;
        count = 0;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = 0.0f;
    }
};

// An edge of the list: each end with the polygon and the polygon edge it was snapped to.
struct UnkNavLink_l29567c {
    UnkNavPolyline_l29567c* startPolygon;
    s32 startIndex;
    CVector2D start;
    UnkNavPolyline_l29567c* endPolygon;
    s32 endIndex;
    CVector2D end;
    UnkNavLink_l29567c* next;  // 0x30
};

struct UnkNavAllocator_l29567c {
    void** vtable;
    void* vf_0x40(u64 size, u64 align, u32 arg) {
        return reinterpret_cast<void* (*)(UnkNavAllocator_l29567c*, u64, u64, u32)>(vtable[0x40 / 8])(this, size,
                                                                                                    align, arg);
    }
};

struct UnkNavLinkList_l29567c {
    u8 pad_0x0[0x8];
    u32 count;
    UnkNavLink_l29567c* head;  // 0x10
    UnkNavLink_l29567c* tail;  // 0x18
    UnkNavAllocator_l29567c* allocator;  // 0x20
    u32 allocArg;  // 0x28
};

static inline UnkShapeTester_A5_b68* Tester() {
    UnkShapeTester_A5_b68* t = g_unk_71726a47a0;
    if (!t) {
        t = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
        fn_7100d5134c(t);
        g_unk_71726a47a0 = t;
    }
    return t;
}

static inline bool Near(f32 v, f32 w, f32 tolerance) {
    return v + tolerance >= w && v - tolerance <= w;
}

static inline bool Near(const CVector2D& v, const CVector2D& w, f32 tolerance) {
    return Near(v.x, w.x, tolerance) && Near(v.y, w.y, tolerance);
}

// Snaps `end` to an end of the edge when it is within 1 of one.
static inline void Snap(CVector2D* end, const UnkSegment2D& edge) {
    if (Near(*end, edge.start, 1.0f))
        *end = edge.start;
    else if (Near(*end, edge.end, 1.0f))
        *end = edge.end;
}

static inline UnkNavPolyline_l29567c* Polygon(UnkNavShape_l29567c* shape, const UnkNavHit_l29567c* hit) {
    return reinterpret_cast<UnkNavPolyline_l29567c*>(
        ResolveHandleToPoolSlot(reinterpret_cast<UnkPool_0x48*>(shape->polygons), hit->handle));
}

// The index of the link with these ends (either way round, within 10), or -1.
static inline s32 Find(const UnkNavLinkList_l29567c* list, const CVector2D& start, const CVector2D& end) {
    s32 index = 0;
    for (UnkNavLink_l29567c* link = list->head; link; link = link->next, index++) {
        if (Near(link->start, start, 10.0f) && Near(link->end, end, 10.0f))
            return index;
        if (Near(link->start, end, 10.0f) && Near(link->end, start, 10.0f))
            return index;
    }
    return -1;
}

static inline void PushBack(UnkNavLinkList_l29567c* list, UnkNavLink_l29567c* link) {
    if (list->count == 0) {
        list->head = link;
        list->tail = link;
    } else {
        list->tail->next = link;
        link->next = nullptr;
        list->tail = link;
    }
    list->count++;
}

void fn_710029567c(void* self, UnkNavArea_l29567c* area, const UnkNavPolyline_l29567c* line,
                   UnkNavLinkList_l29567c* links) {
    UnkNavShape_l29567c* shape = area->shape;
    UnkMatrix44 transform = fn_7100d36a2c(reinterpret_cast<UnkShape*>(area));
    if (!shape || shape->vf_0x18() != 0x10)
        return;

    for (u64 i = 0; i < line->EdgeCount(); i++) {
        const CVector3D* points = line->points;
        const CVector3D* to = i == line->count - 1 ? &points[0] : &points[i] + 1;
        UnkSegment2D seg(reinterpret_cast<const CVector2D&>(points[i]), reinterpret_cast<const CVector2D&>(*to));

        // The segment lengthened by 10 at both ends; its cached length is not known yet.
        CVector2D a = seg.start;
        CVector2D b = seg.end;
        UnkSegment2D longer;
        longer.start = a;
        longer.end = b;
        longer.field_0x10 = -1.0f;
        f32 dx = b.x - a.x;
        f32 dy = b.y - a.y;
        f32 length = sqrtf(dx * dx + dy * dy);
        if (length != 0.0f) {
            dx /= length;
            dy /= length;
        }
        f32 ox = dx * 10.0f;
        f32 oy = dy * 10.0f;
        longer.start.x = a.x - ox;
        longer.start.y = a.y - oy;
        longer.end.x = ox + b.x;
        longer.end.y = oy + b.y;
        longer.field_0x10 = -1.0f;

        UnkNavHits_l29567c hits(longer.start);
        UnkSegmentShape segShape;
        fn_7100d693ac(&segShape, &longer);
        UnkColliderResult result =
            fn_7100d51368(Tester(), shape, &transform, &segShape, reinterpret_cast<const UnkMatrix44&>(segShape.data[8]),
                          reinterpret_cast<UnkShapeFilter_ovb0107*>(&hits), 0);

        UnkNavPolyline_l29567c* startPolygon = nullptr;
        UnkNavPolyline_l29567c* endPolygon = nullptr;
        s32 startIndex = -1;
        s32 endIndex = -1;
        CVector2D start = seg.start;
        CVector2D end = seg.end;
        if (result.bits & 1) {
            for (UnkNavHit_l29567c* hit = hits.hits; hit != &hits.hits[hits.count]; ++hit) {
                bool atStart;
                {
                    CVector2D p;
                    fn_710007b430(&p, &hit->point);
                    atStart = Near(p, seg.start, 1.0f);
                }
                if (atStart) {
                    startPolygon = Polygon(shape, hit);
                    startIndex = (u16)hit->handle;
                    UnkSegment2D edge(EdgeStart(startPolygon, startIndex), EdgeEnd(startPolygon, startIndex));
                    Snap(&start, edge);
                    continue;
                }
                bool atEnd;
                {
                    CVector2D p;
                    fn_710007b430(&p, &hit->point);
                    atEnd = Near(p, seg.end, 1.0f);
                }
                if (atEnd) {
                    endPolygon = Polygon(shape, hit);
                    endIndex = (u16)hit->handle;
                    UnkSegment2D edge(EdgeStart(endPolygon, endIndex), EdgeEnd(endPolygon, endIndex));
                    Snap(&end, edge);
                }
            }
        }

        if (Find(links, start, end) < 0) {
            UnkNavLink_l29567c* link =
                static_cast<UnkNavLink_l29567c*>(links->allocator->vf_0x40(0x38, 8, links->allocArg));
            if (link) {
                link->startPolygon = startPolygon;
                link->startIndex = startIndex;
                link->start = start;
                link->endPolygon = endPolygon;
                link->endIndex = endIndex;
                link->end = end;
                link->next = nullptr;
                PushBack(links, link);
            }
        }
        fn_7100d69400(&segShape);
    }
}
