// UnkTU108 (tools/tu_map.py unit 108): whether a point lies on the inner side of an area's outline,
// within a horizontal radius of an edge; optionally the distance to the nearest such edge and its index.
// Names and layouts are placeholders.
#include <float.h>
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkTU108.h"
#include "types.h"

using base::math::CVector2D;

struct UnkSegment2D_71001262e0;  // a segment's two points (src/game/malloc/fn_71001262e0.cpp)

f32 fn_71001262b4(const UnkSegment2D_71001262e0* seg, const CVector2D* p, CVector2D* closest, f32* t);

struct UnkAreaVertex {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkShakernautArea_b65a4 {
    UnkAreaVertex* vertices;  // 0x0
    u32 count;                // 0x8
    u8 pad_0xc[0x20];
    bool closed;  // 0x2c

    // An open outline has one edge fewer than it has vertices.
    u32 EdgeCount() const { return count - (!closed && count != 0); }
};
static_assert(__builtin_offsetof(UnkShakernautArea_b65a4, closed) == 0x2c, "closed");

bool fn_71001176e8(UnkShakernautArea_b65a4* area, const CVector2D* pos, u32* out, void* filter, u32 flags, f32 radius) {
    f32* best = reinterpret_cast<f32*>(out);
    u32* index = static_cast<u32*>(filter);
    if (best)
        *best = FLT_MAX;

    bool found = false;
    for (u64 i = 0; i < area->EdgeCount(); ++i) {
        const UnkAreaVertex* v = &area->vertices[i];
        const UnkAreaVertex* next = i == area->count - 1 ? &area->vertices[0] : v + 1;
        UnkSegment2D seg(v->pos, next->pos);

        f32 dx = pos->x - seg.start.x;
        f32 dy = pos->y - seg.start.y;
        f32 len = sqrtf(dx * dx + dy * dy);
        if (len != 0.0f) {
            f32 inv = 1.0f / len;
            dx *= inv;
            dy *= inv;
        }
        UnkVec2Result normal = fn_7100125e78(&seg);
        if (dx * normal.x + dy * normal.y < -0.001f)
            continue;
        if (seg.start.x > pos->x + radius || seg.end.x < pos->x - radius)
            continue;

        if (best) {
            f32 dist;
            if (flags == 0)
                dist = fn_71001262b4(reinterpret_cast<const UnkSegment2D_71001262e0*>(&seg), pos, nullptr, nullptr);
            else if (flags == 1)
                dist = fabsf((seg.start.y + seg.end.y) * 0.5f - pos->y);
            else
                dist = -1.0f;
            if (*best > dist)
                *best = dist;
        }
        found = true;
        if (index)
            *index = i;
        // Without a distance or an index to report, the first edge in reach is enough.
        if (!best && !index)
            break;
    }
    return found;
}
