// Unit 365 ("navmesh::CNavMeshGenerator"): whether an edge crosses any edge of a list somewhere
// other than at shared endpoints. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkEdgeCursor_b0228;

struct UnkContact_710012be70 {
    CVector3D point;
    CVector3D normal_a;
    CVector3D normal_b;
    u32 field_0x24;
    u32 flags;

    void SetPoint(const CVector3D& p) {
        point = p;
        flags |= 8;
    }
    void SetNormalA(const UnkVec2Result& n) {
        normal_a.x = n.x;
        normal_a.y = n.y;
        normal_a.z = 0.0f;
        flags |= 0x20;
    }
    void SetNormalB(const UnkVec2Result& n) {
        normal_b.x = n.x;
        normal_b.y = n.y;
        normal_b.z = 0.0f;
        flags |= 0x10;
    }
};

UnkSegment2D fn_7100289128(UnkEdgeCursor_b0228* self);
bool fn_710012be70(const UnkSegment2D* a, const UnkSegment2D* b, UnkContact_710012be70* out,
                   bool collinear, f32 tolerance);

// A list node: the edge cursor it holds, then the next node.
struct UnkEdgeNode_2a3dbc {
    u8 cursor[0x30];
    UnkEdgeNode_2a3dbc* next;  // 0x30

    UnkEdgeCursor_b0228* Cursor() { return reinterpret_cast<UnkEdgeCursor_b0228*>(cursor); }
};

struct UnkEdgeList_2a3dbc {
    u8 pad_0x0[0x10];
    UnkEdgeNode_2a3dbc* head;  // 0x10
};

struct UnkOwner_2a3dbc;

// Whether `p` lies within one unit of `q` on both axes.
static inline bool IsNear(const CVector2D& q, const CVector2D& p) {
    const f32* qc = &q.x;
    const f32* pc = &p.x;
    for (s32 i = 0; i < 2; i++) {
        if (!(qc[i] + 1.0f >= pc[i] && qc[i] - 1.0f <= pc[i])) {
            return false;
        }
    }
    return true;
}

// Whether the two edges cross somewhere other than near an endpoint of `edge`.
static inline bool CrossesAwayFromEnds(UnkEdgeCursor_b0228* edge, UnkEdgeNode_2a3dbc* node) {
    UnkSegment2D other = fn_7100289128(node->Cursor());
    UnkSegment2D seg = fn_7100289128(edge);
    if (IsNear(seg.start, other.start) || IsNear(seg.start, other.end) ||
        IsNear(seg.end, other.start) || IsNear(seg.end, other.end)) {
        return false;
    }
    UnkContact_710012be70 contact = {};
    if (!fn_710012be70(&seg, &other, &contact, false, 0.0f)) {
        return false;
    }
    CVector2D point;
    fn_710007b430(&point, &contact.point);
    return !IsNear(point, seg.start) && !IsNear(point, seg.end);
}

bool fn_71002a3dbc(UnkOwner_2a3dbc* self, UnkEdgeCursor_b0228* edge, UnkEdgeList_2a3dbc* list) {
    for (UnkEdgeNode_2a3dbc* node = list->head; node != nullptr; node = node->next) {
        if (CrossesAwayFromEnds(edge, node)) {
            return true;
        }
    }
    return false;
}
