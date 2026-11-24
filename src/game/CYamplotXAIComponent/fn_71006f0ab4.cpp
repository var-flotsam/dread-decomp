// CYamplotXAIComponent's unit: the edge before the one a polygon handle names (the last edge for edge 0).
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkTU109.h"
#include "types.h"

using base::math::CVector2D;

struct UnkPolygonVertex {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkPolygon2D {
    UnkPolygonVertex* vertices;
    u32 count;
    u8 unk_0xc[0x20];
    u8 closed;
    bool box_dirty;  // 0x2d: the box below is stale
    u8 unk_0x2e[0x2];
    CVector2D box_min;  // 0x30
    CVector2D box_max;  // 0x38
    u8 unk_0x40[0x8];
};
static_assert(sizeof(UnkPolygon2D) == 0x48, "UnkPolygon2D layout");

static inline u32 EdgeCount(const UnkPolygon2D* p) {
    return p->count - (!p->closed & (p->count != 0));
}

static inline UnkSegment2D EdgeAt(const UnkPolygon2D* p, u32 i) {
    const UnkPolygonVertex* v = &p->vertices[i];
    return UnkSegment2D(v->pos, (i == p->count - 1 ? p->vertices : v + 1)->pos);
}

// NON_MATCHING(functional): Control flow differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
UnkSegment2D fn_71006f0ab4(UnkPool_0x48* pool, u32 handle) {
    const UnkPolygon2D* polygon = reinterpret_cast<const UnkPolygon2D*>(ResolveHandleToPoolSlot(pool, handle));
    u32 edge = handle & 0xffff;
    if (edge != 0 || polygon->closed) {
        u32 last = edge != 0 ? edge : EdgeCount(polygon);
        return EdgeAt(polygon, last - 1);
    }
    UnkSegment2D first = EdgeAt(polygon, 0);
    return UnkSegment2D(first.start, first.start);
}
