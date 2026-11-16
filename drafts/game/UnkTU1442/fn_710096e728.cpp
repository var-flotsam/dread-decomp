// Unit 1442: the edge of a 2D polygon next to edge `i`, the following one when `forward` and the previous
// one otherwise; past the end of an open polygon it is the degenerate segment at that end.
// Names and layouts are placeholders.
#include <new>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;

struct UnkPolygonVertex_l96e728 {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkPolygon2D_l96e728 {
    UnkPolygonVertex_l96e728* vertices;
    u32 count;
    u8 unk_0xc[0x20];
    u8 closed;  // 0x2c

    u32 EdgeCount() const { return count - (!closed & (count != 0)); }
    // Edge `i`: from its vertex to the next one, wrapping to the first.
    UnkSegment2D Edge(u32 i) const {
        return UnkSegment2D(vertices[i].pos, (i + 1 == count ? vertices : &vertices[i + 1])->pos);
    }
};

UnkSegment2D fn_710096e728(const UnkPolygon2D_l96e728* polygon, u32 i, bool forward) {
    bool open = !polygon->closed;
    if (!forward) {
        if (i == 0 && open) {
            UnkSegment2D first = polygon->Edge(0);
            return UnkSegment2D(first.start, first.start);
        }
        return polygon->Edge((i != 0 ? i : polygon->EdgeCount()) - 1);
    }
    if (open && i >= polygon->EdgeCount() - 1) {
        UnkSegment2D last = polygon->Edge(i);
        return UnkSegment2D(last.end, last.end);
    }
    u32 next = i == polygon->EdgeCount() - 1 ? 0 : i + 1;
    return polygon->Edge(next);
}
