// Unit 365 ("navmesh::CNavMeshGenerator"): traces an outline from a link between two polygon
// edges, walking each polygon's edges and crossing over at the links of a list until it is back at
// the start. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkTU108.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkPointSet_b61;
void* fn_7100118950(UnkPointSet_b61* self, u32 index);

struct UnkTilePoint {
    f32 x, y, z;
};
struct UnkTileOutline {
    base::global::CRntVector<UnkTilePoint> points;
    f32 field_0x28;
    bool field_0x2c;
    bool field_0x2d;
    base::math::CVector2D min;
    base::math::CVector2D max;
    u32 field_0x40;

    UnkTileOutline()
        : points(7), field_0x28(0.0f), field_0x2c(false), field_0x2d(true), min{3.4028235e38f, 3.4028235e38f},
          max{-3.4028235e38f, -3.4028235e38f}, field_0x40(0) {}
    ~UnkTileOutline() { points.Free(); }
};
static_assert(sizeof(UnkTileOutline) == 0x48, "UnkTileOutline placeholder layout");

// One edge of a polygon: the polygon and the index of the edge's first point.
struct UnkEdgeRef_2a45cc {
    UnkTileOutline* polygon;
    u32 index;

    bool operator==(const UnkEdgeRef_2a45cc& o) const { return polygon == o.polygon && index == o.index; }
};

// Two edges that meet, and the next link of the list.
struct UnkEdgeLink_2a45cc {
    UnkEdgeRef_2a45cc a;  // 0x0
    UnkEdgeRef_2a45cc b;  // 0x10
    u8 pad_0x20[8];
    UnkEdgeLink_2a45cc* next;  // 0x28
};

struct UnkEdgeLinks_2a45cc {
    u8 pad_0x0[0x10];
    UnkEdgeLink_2a45cc* head;  // 0x10
};

struct UnkOwner_2a45cc;

static inline UnkSegment2D EdgeOf(const UnkEdgeRef_2a45cc& edge) {
    UnkTileOutline* polygon = edge.polygon;
    UnkTilePoint* start = &polygon->points.data[edge.index];
    UnkTilePoint* end =
        polygon->points.count - 1 == edge.index ? polygon->points.data : &polygon->points.data[edge.index + 1];
    return UnkSegment2D(*reinterpret_cast<CVector2D*>(start), *reinterpret_cast<CVector2D*>(end));
}

// The index of the edge after `index`: an open polygon has one edge fewer than it has points.
static inline u32 NextEdge(const UnkTileOutline* polygon, u32 index) {
    bool open = !polygon->field_0x2c;
    u32 count = polygon->points.count;
    u32 edges = count - (open && count != 0);
    return index == edges - 1 ? 0 : index + 1;
}

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

static inline void AddPoint(UnkTileOutline* outline, const CVector2D& p) {
    UnkTilePoint point;
    point.x = p.x;
    point.y = p.y;
    point.z = 0.0f;
    fn_7100118b94(outline, reinterpret_cast<UnkVec3*>(&point));
}

bool fn_71002a45cc(UnkOwner_2a45cc* self, UnkEdgeLinks_2a45cc* links, UnkTileOutline* outline,
                   UnkEdgeLink_2a45cc* from) {
    outline->points.count = 0;
    outline->field_0x28 = 0.0f;
    outline->field_0x2c = false;
    outline->field_0x2d = true;

    UnkEdgeRef_2a45cc start = from->a;
    UnkSegment2D first = EdgeOf(start);
    AddPoint(outline, first.end);

    UnkEdgeRef_2a45cc edge = from->b;
    bool stay = true;  // the first edge, and one just crossed to, are not crossed from
    bool crossed;
    do {
        u32 next = NextEdge(edge.polygon, edge.index);
        UnkSegment2D seg = EdgeOf(edge);
        const CVector2D* last =
            static_cast<const CVector2D*>(fn_7100118950(reinterpret_cast<UnkPointSet_b61*>(outline),
                                                        outline->points.count - 1));
        if (!IsNear(*last, seg.end)) {
            AddPoint(outline, seg.end);
        }
        crossed = false;
        if (!stay) {
            for (UnkEdgeLink_2a45cc* link = links->head; link != nullptr; link = link->next) {
                if (link->a == edge || link->b == edge) {
                    edge = link->a == edge ? link->b : link->a;
                    crossed = true;
                    break;
                }
            }
        }
        if (!crossed) {
            edge.index = next;
        }
        stay = crossed || edge == start;
    } while (!(edge == start) || crossed);

    fn_7100119f98(outline, true);
    return false;
}
