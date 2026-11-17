// UnkTU2702 (CAIComponent's unit): the edge of a path at an index, with each end pulled in along the edge
// by the path's shift where the neighbouring edge turns more sharply than the path's limit allows.
// Names and layouts are placeholders.
#include <math.h>

#include "base/math/UnkSegment2D.h"
#include "game/malloc.h"
#include "types.h"

using base::math::CVector2D;

struct UnkPathPoint_l11632c {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkPath_l11632c {
    u8 pad_0x0[0x8];
    f32 limit;  // 0x8: the cosine above which a corner is left alone
    f32 shift;  // 0xc
    UnkPathPoint_l11632c* points;  // 0x10
    u32 count;                     // 0x18
    u8 pad_0x1c[0x20];
    bool closed;  // 0x3c

    // The edge from point i to the next one (the first after the last).
    UnkSegment2D Edge(u32 i) const {
        UnkPathPoint_l11632c* p = points;
        const UnkPathPoint_l11632c* next = count - 1 == i ? p : &p[i + 1];
        return UnkSegment2D(p[i].pos, next->pos);
    }

    u32 EdgeCount() const { return closed || count == 0 ? count : count - 1; }

    // The edge before edge i; at the start of an open path, the first point alone.
    UnkSegment2D PrevEdge(u32 i) const {
        u32 prev = (i == 0 ? EdgeCount() : i) - 1;
        if (i == 0 && !closed) {
            UnkSegment2D first = Edge(0);
            return UnkSegment2D(first.start, first.start);
        }
        return Edge(prev);
    }

    // The edge after edge i; at the end of an open path, the last point alone.
    UnkSegment2D NextEdge(u32 i) const {
        u32 next = i == EdgeCount() - 1 ? 0 : i + 1;
        if (!closed && i >= EdgeCount() - 1) {
            UnkSegment2D last = Edge(i);
            return UnkSegment2D(last.end, last.end);
        }
        return Edge(next);
    }
};

struct UnkDir_l11632c {
    f32 x, y;
};

static inline UnkDir_l11632c Direction(const UnkSegment2D& seg) {
    UnkDir_l11632c d;
    d.x = seg.end.x - seg.start.x;
    d.y = seg.end.y - seg.start.y;
    f32 length = sqrtf(d.x * d.x + d.y * d.y);
    if (length != 0.0f) {
        f32 inv = 1.0f / length;
        d.x *= inv;
        d.y *= inv;
    }
    return d;
}

static inline f32 Dot(const UnkDir_l11632c& a, const UnkDir_l11632c& b) {
    return a.x * b.x + a.y * b.y;
}

void fn_710111632c(UnkPath_l11632c* self, const u32* index, UnkSegment2D* out) {
    u32 i = *index;
    {
        UnkSegment2D edge = self->Edge(i);
        fn_7100125b44(out, &edge);
    }
    if (!(self->shift > 0.0f))
        return;
    UnkSegment2D prev = self->PrevEdge(i);
    UnkSegment2D next = self->NextEdge(i);
    UnkDir_l11632c dir = Direction(*out);
    UnkDir_l11632c prevDir = Direction(prev);
    UnkDir_l11632c nextDir = Direction(next);
    f32 limit = self->limit;
    CVector2D start = out->start;
    if (limit < 0.0f || Dot(dir, prevDir) <= limit) {
        start.x = start.x + dir.x * self->shift;
        start.y = start.y + dir.y * self->shift;
    }
    CVector2D end = out->end;
    if (limit < 0.0f || Dot(dir, nextDir) <= limit) {
        end.x = end.x - dir.x * self->shift;
        end.y = end.y - dir.y * self->shift;
    }
    out->start = start;
    out->end = end;
    out->field_0x10 = -1.0f;
}
