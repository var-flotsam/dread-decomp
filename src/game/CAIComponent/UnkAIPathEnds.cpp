// In CAIComponent's unit (unit 2702 of tools/tu_map.py): the first and the last leg of an AI path
// (its points at +0x68, 12 bytes each, the count at +0x70; +0x94 set when the path is a loop). A
// path that is not a loop has one leg fewer than points. Each builds the leg's segment
// (base/math/UnkSegment2D.h) and hands back one word of it: the first leg its first word, the last
// leg its third. Names and layouts are placeholders.
#include "base/math/UnkSegment2D.h"

using base::math::CVector2D;

struct UnkAIPathPoint {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkAIPath {
    u8 pad_0x0[0x68];
    UnkAIPathPoint* points;
    u32 count;  // 0x70
    u8 pad_0x74[0x20];
    u8 loop;  // 0x94, a byte flag
};

// Whether the path is open (not a loop, and not empty): computed without branching, the flag first.
static inline u32 IsOpen(const UnkAIPath* self, u32 n) {
    return (u32)(self->loop == 0) & (u32)(n != 0);
}

bool fn_71010fc31c(UnkAIPath* self, u32* out) {
    u32 n = self->count;
    if (n == IsOpen(self, n))
        return false;
    const UnkAIPathPoint* a = self->points;
    const UnkAIPathPoint* b = n != 1 ? a + 1 : a;
    UnkSegment2D seg(a->pos, b->pos);
    *out = *reinterpret_cast<const u32*>(&seg);
    return true;
}

// NON_MATCHING(minor): the original keeps a second, unreachable test of the leg count (storing 0 and
// returning true) and its registers differ; the behaviour is the same.
bool fn_71010fc284(UnkAIPath* self, u32* out) {
    u32 n = self->count;
    u32 open = IsOpen(self, n);
    u32 legs = n - open;
    if (legs == 0)
        return false;
    const UnkAIPathPoint* points = self->points;
    const UnkAIPathPoint* end = open ? &points[legs] : points;
    UnkSegment2D seg(points[legs - 1].pos, end->pos);
    *out = *reinterpret_cast<const u32*>(reinterpret_cast<const u8*>(&seg) + 8);
    return true;
}
