// Unit 108: finds the polygon edge nearest a point (fn_7100117ba0) and, when the closest point is at
// an edge's end, reports the neighbouring edge and which end it shares. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;

struct UnkPolygon2D_l118124 {
    void* vertices;
    u32 count;
    u8 unk_0xc[0x20];
    u8 closed;
};

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

static inline s32 EdgeCount(const UnkPolygon2D_l118124* p) {
    return p->count - (!p->closed & (p->count != 0));
}

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
            e = EdgeCount(polygon);
        *out_edge = e - 1;
        const u32 one = 0x3f800000;
        __builtin_memcpy(out_end, &one, 4);
    } else {
        if (!(hi >= 1.0f && lo <= 1.0f))
            return false;
        s32 e = *edge;
        *out_edge = (EdgeCount(polygon) - 1 == e) ? 0 : e + 1;
        const u32 zero = 0;
        __builtin_memcpy(out_end, &zero, 4);
    }
    return true;
}
