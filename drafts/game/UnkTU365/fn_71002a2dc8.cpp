// Unit 365 (navmesh::CNavMeshGenerator): adds the start of a neighbour polygon's edge to the polygon
// being built, unless a point is already there, and moves the edge's links (both sides, and every
// edge pair naming it) over to the new edge. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkTU108.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkUnit108List_A1_b73;
struct UnkUnit108Point_A1_b73;
bool fn_7100118b1c(const UnkUnit108List_A1_b73* self, const UnkUnit108Point_A1_b73* p, f32 r);

struct UnkNavPoly_l2a2dc8;

// One side of an edge: the polygon and the index of its edge.
struct UnkEdgeRef_l2a2dc8 {
    UnkNavPoly_l2a2dc8* poly = nullptr;
    u32 index = 0;
};

struct UnkEdgeLink_l2a2dc8 {
    UnkEdgeRef_l2a2dc8 ref;
    UnkEdgeLink_l2a2dc8* next = nullptr;
};

// The polygon across each edge, as a singly linked list.
struct UnkEdgeLinks_l2a2dc8 {
    u32 count;
    UnkEdgeLink_l2a2dc8* head;
    UnkEdgeLink_l2a2dc8* tail;
    base::IAllocator* allocator;
    u32 flags;

    // The link at `index`, or null past the end of the list.
    UnkEdgeLink_l2a2dc8* Find(u32 index) const {
        UnkEdgeLink_l2a2dc8* link;
        for (link = head; link; link = link->next) {
            if (index == 0)
                break;
            --index;
        }
        return link;
    }

    UnkEdgeLink_l2a2dc8& operator[](u32 index) const {
        UnkEdgeLink_l2a2dc8* link = head;
        for (u32 i = 0; i < index; ++i)
            link = link->next;
        return *link;
    }

    void PushBack() {
        void* mem = allocator->vf_0x40(sizeof(UnkEdgeLink_l2a2dc8), 8, flags);
        if (mem) {
            UnkEdgeLink_l2a2dc8* link = new (mem) UnkEdgeLink_l2a2dc8;
            if (count != 0) {
                tail->next = link;
                link->next = nullptr;
                ++count;
                tail = link;
            } else {
                head = link;
                tail = link;
                count = 1;
            }
        }
    }
};

struct UnkNavPoly_l2a2dc8 {
    base::global::CRntVector<CVector3D> points;
    u8 pad_0x28[4];
    bool closed;  // 0x2c
    u8 pad_0x2d[0x23];
    UnkEdgeLinks_l2a2dc8 links;  // 0x50

    u32 NumEdges() const {
        u32 n = points.count;
        return closed || n == 0 ? n : n - 1;
    }

};
static_assert(__builtin_offsetof(UnkNavPoly_l2a2dc8, links) == 0x50, "layout");

// Two edges that meet, as a singly linked list.
struct UnkEdgePair_l2a2dc8 {
    UnkEdgeRef_l2a2dc8 a;
    UnkEdgeRef_l2a2dc8 b;
    UnkEdgePair_l2a2dc8* next;
};

struct UnkEdgePairs_l2a2dc8 {
    u8 pad_0x0[0x10];
    UnkEdgePair_l2a2dc8* head;
};

// The end of edge `index` of the polygon (its next point, or the first one after the last).
inline CVector2D EdgeEnd(const UnkNavPoly_l2a2dc8* poly, u32 index) {
    const base::global::CRntVector<CVector3D>& points = poly->points;
    const CVector3D* start = &points.data[index];
    const CVector3D* end = points.count - 1 == index ? &points.data[0] : &points.data[index + 1];
    UnkSegment2D segment(reinterpret_cast<const CVector2D&>(*start), reinterpret_cast<const CVector2D&>(*end));
    return segment.end;
}

void fn_71002a2dc8(void* self, const UnkEdgeRef_l2a2dc8* edge, UnkNavPoly_l2a2dc8* poly, u32 index,
                   UnkEdgePairs_l2a2dc8* pairs, s32* next_index, UnkNavPoly_l2a2dc8* out) {
    bool added = false;
    CVector2D at = EdgeEnd(edge->poly, index);
    if (!fn_7100118b1c(reinterpret_cast<const UnkUnit108List_A1_b73*>(out),
                       reinterpret_cast<const UnkUnit108Point_A1_b73*>(&at), 1.0f)) {
        CVector3D point = {at.x, at.y, 0.0f};
        fn_7100118b94(out, reinterpret_cast<UnkVec3*>(&point));
        if (out->NumEdges() != 0)
            out->links.PushBack();
        added = true;
    }

    if (*next_index >= 0 && edge->index != index) {
        UnkEdgeLink_l2a2dc8* link = edge->poly->links.Find(index);
        UnkNavPoly_l2a2dc8* other = link->ref.poly;
        if (other) {
            if (out->links.count > (u32)*next_index) {
                UnkEdgeRef_l2a2dc8& ref = out->links[*next_index].ref;
                ref.poly = other;
                ref.index = link->ref.index;
            }
            if (other->links.count > link->ref.index) {
                UnkEdgeRef_l2a2dc8& ref = other->links[link->ref.index].ref;
                ref.poly = out;
                ref.index = *next_index;
            }
            for (UnkEdgePair_l2a2dc8* pair = pairs->head; pair; pair = pair->next) {
                if (pair->a.poly == poly && pair->a.index == index) {
                    pair->a.poly = out;
                    pair->a.index = *next_index;
                }
                if (pair->b.poly == poly && pair->b.index == index) {
                    pair->b.poly = out;
                    pair->b.index = *next_index;
                }
            }
        }
    }

    if (added)
        ++*next_index;
}
