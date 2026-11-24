// Unit 365 ("navmesh::CNavMeshGenerator"): adds an edge to a list of edges unless one already lies along
// it, first splitting it and the listed edges at every point where they cross. Names and layouts are
// placeholders.
#include <math.h>
#include <new>
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::global::CRntVector;
using base::math::CVector2D;
using base::math::CVector3D;

struct UnkContact_710012be70;
bool fn_710012be70(const UnkSegment2D* a, const UnkSegment2D* b, UnkContact_710012be70* out, bool collinear,
                   f32 tolerance);
struct UnkNavRecord_l28d970;
UnkNavRecord_l28d970* fn_710028d970(base::global::CRntVector<UnkNavRecord_l28d970>* self, u32 index);

// What fn_710012be70 fills on a hit.
struct UnkContact_l28ce1c {
    CVector3D point;     // 0x0
    CVector3D normal_a;  // 0xc
    CVector3D normal_b;  // 0x18
    u32 field_0x24;      // 0x24
    u32 flags;           // 0x28
};

// One end of an edge: the polygon edge it lies on (none: index 0xffff), and its point.
struct UnkEdgeEnd_l28ce1c {
    void* polygon;  // 0x0
    u32 index;      // 0x8
    CVector2D pos;  // 0xc

    UnkEdgeEnd_l28ce1c(const CVector2D& p) : polygon(nullptr), index(0xffff), pos(p) {}
    UnkEdgeEnd_l28ce1c(const UnkEdgeEnd_l28ce1c& o) : polygon(o.polygon), index(o.index) {
        pos.x = o.pos.x;
        pos.y = o.pos.y;
    }
};

struct UnkEdge_l28ce1c {
    UnkEdgeEnd_l28ce1c start;  // 0x0
    UnkEdgeEnd_l28ce1c end;    // 0x18

    UnkEdge_l28ce1c(const UnkEdgeEnd_l28ce1c& s, const UnkEdgeEnd_l28ce1c& e) : start(s), end(e) {}
    UnkEdge_l28ce1c(const UnkEdge_l28ce1c& o) : start(o.start), end(o.end) {}
};
static_assert(sizeof(UnkEdge_l28ce1c) == 0x30, "UnkEdge_l28ce1c layout");

struct UnkEdgeNode_l28ce1c {
    UnkEdge_l28ce1c edge;       // 0x0
    UnkEdgeNode_l28ce1c* next;  // 0x30
};

struct UnkEdgeList_l28ce1c {
    u8 pad_0x0[8];
    u32 count;                      // 0x8
    UnkEdgeNode_l28ce1c* head;      // 0x10
    UnkEdgeNode_l28ce1c* tail;      // 0x18
    base::IAllocator* allocator;  // 0x20
    u32 flags;                      // 0x28

    void PushBack(const UnkEdge_l28ce1c& edge) {
        UnkEdgeNode_l28ce1c* node =
            static_cast<UnkEdgeNode_l28ce1c*>(allocator->vf_0x40(sizeof(UnkEdgeNode_l28ce1c), 8, flags));
        if (!node)
            return;
        new (&node->edge) UnkEdge_l28ce1c(edge);
        node->next = nullptr;
        if (count == 0) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            node->next = nullptr;
            tail = node;
        }
        count = count + 1;
    }
};

static void Remove(UnkEdgeList_l28ce1c* list, UnkEdgeNode_l28ce1c* node) {
    UnkEdgeNode_l28ce1c* prev = nullptr;
    for (UnkEdgeNode_l28ce1c* it = list->head; it; prev = it, it = it->next) {
        if (it != node)
            continue;
        if (prev)
            prev->next = it->next;
        else
            list->head = it->next;
        if (list->tail == node)
            list->tail = prev;
        if (--list->count == 0) {
            list->head = nullptr;
            list->tail = nullptr;
        }
        list->allocator->vf_0x48(node);
        return;
    }
}

static inline void Append(CRntVector<UnkEdge_l28ce1c>& edges, const UnkEdge_l28ce1c& edge) {
    u32 n = edges.count;
    u32 cap = edges.capacity;
    if (cap <= n && edges.step != 0)
        cap = edges.GrownCapacity(n);
    if ((s32)n >= (s32)cap)
        for (;;) {
        }
    edges.Reserve(cap);
    new (&edges.data[edges.count]) UnkEdge_l28ce1c(edge);
    ++edges.count;
}

static inline UnkEdge_l28ce1c* EdgeAt(CRntVector<UnkEdge_l28ce1c>& edges, u32 index) {
    return reinterpret_cast<UnkEdge_l28ce1c*>(
        fn_710028d970(reinterpret_cast<CRntVector<UnkNavRecord_l28d970>*>(&edges), index));
}

static inline bool IsNear(f32 v, f32 target) { return v - 1.0f <= target && v + 1.0f >= target; }

static inline bool IsNear(const CVector2D& p, const CVector2D& target) {
    return IsNear(p.x, target.x) && IsNear(p.y, target.y);
}

static inline bool IsSameDirection(f32 x, f32 y, f32 tx, f32 ty) {
    return x + 0.001f >= tx && x - 0.001f <= tx && (y + 0.001f >= ty & y - 0.001f <= ty);
}

static inline void Normalize(f32& x, f32& y) {
    f32 len = sqrtf(x * x + y * y);
    if (len != 0.0f) {
        f32 inv = 1.0f / len;
        x *= inv;
        y *= inv;
    }
}

void fn_710028ce1c(void* self, const UnkEdge_l28ce1c* edge, UnkEdgeList_l28ce1c* list) {
    UnkSegment2D seg(edge->start.pos, edge->end.pos);
    f32 dir_x = seg.end.x - seg.start.x;
    f32 dir_y = seg.end.y - seg.start.y;
    Normalize(dir_x, dir_y);

    bool covered = false;
    for (UnkEdgeNode_l28ce1c* node = list->head; node; node = node->next) {
        UnkEdge_l28ce1c other(node->edge);
        UnkSegment2D other_seg(other.start.pos, other.end.pos);
        f32 x = other_seg.end.x - other_seg.start.x;
        f32 y = other_seg.end.y - other_seg.start.y;
        Normalize(x, y);
        if (IsNear(other_seg.start, seg.start))
            covered |= IsSameDirection(x, y, dir_x, dir_y);
        else if (IsNear(other_seg.start, seg.end))
            covered |= IsSameDirection(x, y, -dir_x, -dir_y);
        else if (IsNear(other_seg.end, seg.end))
            covered |= IsSameDirection(-x, -y, -dir_x, -dir_y);
        else if (IsNear(other_seg.end, seg.start))
            covered |= IsSameDirection(-x, -y, dir_x, dir_y);
    }
    if (covered)
        return;

    CRntVector<UnkEdge_l28ce1c> pieces(0xf);
    Append(pieces, *edge);
    for (u32 i = 0; i < pieces.count;) {
        UnkEdge_l28ce1c piece(*EdgeAt(pieces, i));
        bool split = false;
        for (UnkEdgeNode_l28ce1c* node = list->head; node; node = node->next) {
            UnkEdge_l28ce1c other(node->edge);
            UnkSegment2D other_seg(other.start.pos, other.end.pos);
            UnkSegment2D piece_seg(piece.start.pos, piece.end.pos);
            UnkContact_l28ce1c contact = {};
            if (!fn_710012be70(&piece_seg, &other_seg, reinterpret_cast<UnkContact_710012be70*>(&contact), false,
                               0.0f))
                continue;
            CVector2D cross;
            fn_710007b430(&cross, &contact.point);
            // A crossing at an end of either edge splits nothing.
            if (IsNear(cross, piece_seg.start))
                continue;
            if (IsNear(cross, piece_seg.end))
                continue;
            if (IsNear(cross, other_seg.start))
                continue;
            if (IsNear(cross, other_seg.end))
                continue;

            Remove(list, node);
            list->PushBack(UnkEdge_l28ce1c(other.start, UnkEdgeEnd_l28ce1c(cross)));
            list->PushBack(UnkEdge_l28ce1c(UnkEdgeEnd_l28ce1c(cross), other.end));
            new (EdgeAt(pieces, i)) UnkEdge_l28ce1c(piece.start, UnkEdgeEnd_l28ce1c(cross));
            Append(pieces, UnkEdge_l28ce1c(UnkEdgeEnd_l28ce1c(cross), piece.end));
            split = true;
            break;
        }
        if (!split)
            ++i;
    }

    for (u32 i = 0; i < pieces.count; ++i)
        list->PushBack(*EdgeAt(pieces, i));
    pieces.Free();
}
