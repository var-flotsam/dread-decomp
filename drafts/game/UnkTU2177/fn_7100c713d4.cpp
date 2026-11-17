// Unit 2177: finds the entry of a list that lies at a given entry's position (within a tolerance) and
// whose point fifty units along its own path segment is as far from the owner as the given entry's
// is; stores its index. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/malloc.h"
#include "types.h"

using base::math::CVector2D;

extern const f32 g_unk_7101647774;

struct UnkPathPoint_lc713d4 {
    CVector2D pos;
    u32 field_0x8;
};

// A path: its points, and whether it is closed (the last segment runs back to the first point).
struct UnkPath_lc713d4 {
    UnkPathPoint_lc713d4* points;  // 0x0
    u32 count;                     // 0x8
    u8 pad_0xc[0x20];
    u8 closed;                     // 0x2c

    // The segment of the path at `index`: from that point to the next, wrapping round at the end; the
    // last index of an open path gives the segment ending at its last point.
    UnkSegment2D Segment(u32 index) const {
        u32 open = closed == 0;
        u32 last = count - (open & (count != 0));
        const UnkPathPoint_lc713d4* a;
        const UnkPathPoint_lc713d4* b;
        if (index == last) {
            a = &points[last - 1];
            b = (closed == 0 && count != 0) ? &points[last] : &points[0];
        } else {
            a = &points[index];
            b = (count - 1 == index) ? &points[0] : &points[index + 1];
        }
        return UnkSegment2D(a->pos, b->pos);
    }
};

struct UnkPathEntry_lc713d4 {
    CVector2D pos;           // 0x0
    u8 pad_0x8[8];
    UnkPath_lc713d4* path;   // 0x10
    u32 index;               // 0x18
    u8 pad_0x1c[0xc];
};
static_assert(sizeof(UnkPathEntry_lc713d4) == 0x28, "UnkPathEntry_lc713d4 size");

struct UnkPathEntryList_lc713d4 {
    u32 count;
    UnkPathEntry_lc713d4 items[1];  // capacity unknown
};

struct UnkPathFinder_lc713d4 {
    u8 pad_0x0[0x150];
    CVector2D pos;  // 0x150
};

// A segment with no points and its length not yet computed.
static inline void Clear(UnkSegment2D* seg) {
    seg->start = {};
    seg->end = {};
    seg->field_0x10 = -1.0f;
}

static inline void Assign(UnkSegment2D* dst, const UnkSegment2D& src) {
    fn_7100125b44(dst, &src);
}

// The unit direction of a segment (zero for an empty one).
static inline CVector2D UnitDirection(const UnkSegment2D& seg) {
    f32 dy = seg.end.y - seg.start.y;
    f32 dx = seg.end.x - seg.start.x;
    f32 length = sqrtf(dx * dx + dy * dy);
    if (length != 0.0f) {
        f32 inverse = 1.0f / length;
        dx *= inverse;
        dy *= inverse;
    }
    return {dx, dy};
}

bool fn_7100c713d4(UnkPathFinder_lc713d4* self, UnkPathEntryList_lc713d4* list, const UnkPathEntry_lc713d4* given,
                   s32* out) {
    s32 i = 0;
    UnkPathEntry_lc713d4* end = list->items + list->count;
    for (UnkPathEntry_lc713d4* entry = list->items; entry != end; ++entry, ++i) {
        f32 tolerance = g_unk_7101647774;
        f32 x = entry->pos.x;
        if (!(x - tolerance <= given->pos.x && x + tolerance >= given->pos.x))
            continue;
        f32 y = entry->pos.y;
        if (!(y - tolerance <= given->pos.y && tolerance + y >= given->pos.y))
            continue;
        UnkPath_lc713d4* path = entry->path;
        u32 index = entry->index;

        UnkSegment2D givenSeg;
        Clear(&givenSeg);
        Assign(&givenSeg, given->path->Segment(given->index));
        CVector2D dir = UnitDirection(givenSeg);
        f32 ax = dir.x * 50.0f + given->pos.x - self->pos.x;
        f32 ay = dir.y * 50.0f + given->pos.y - self->pos.y;
        f32 givenDistSq = ax * ax + ay * ay;

        UnkSegment2D seg;
        Clear(&seg);
        Assign(&seg, path->Segment(index));
        dir = UnitDirection(seg);
        f32 bx = x + dir.x * 50.0f - self->pos.x;
        f32 by = dir.y * 50.0f + y - self->pos.y;
        f32 distSq = bx * bx + by * by;

        if (distSq <= givenDistSq + 1e-7f && givenDistSq - 1e-7f <= distSq) {
            *out = i;
            return true;
        }
    }
    return false;
}
