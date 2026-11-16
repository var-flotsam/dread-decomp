// Unit 108: whether two polygons have the same points in the same order within a small tolerance,
// starting anywhere along the first one. Names and layouts are placeholders.
#include "types.h"

struct UnkPathCopy_b0255;

struct UnkPolyPoint_l118ef0 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkPolygon_l118ef0 {
    UnkPolyPoint_l118ef0* points;
    u32 count;
    u8 pad_0xc[0x20];
    u8 closed;

    u32 EdgeCount() const { return count - ((count != 0) & !closed); }
    const UnkPolyPoint_l118ef0* begin() const { return points; }
    const UnkPolyPoint_l118ef0* end() const { return points + count; }
};

static inline bool NearlyEqual(f32 a, f32 b) {
    return a - 0.001f <= b && a + 0.001f >= b;
}

static inline bool NearlyEqual(const UnkPolyPoint_l118ef0& a, const UnkPolyPoint_l118ef0& b) {
    return NearlyEqual(a.x, b.x) && NearlyEqual(a.y, b.y);
}

bool fn_7100118ef0(UnkPathCopy_b0255* path, void* target) {
    const UnkPolygon_l118ef0* self = (const UnkPolygon_l118ef0*)path;
    const UnkPolygon_l118ef0* other = (const UnkPolygon_l118ef0*)target;
    u32 edges = other->EdgeCount();
    if (edges != self->EdgeCount())
        return false;
    if (edges == 0)
        return true;
    for (const UnkPolyPoint_l118ef0* start = self->begin(); start != self->end(); ++start) {
        if (!NearlyEqual(*start, other->points[0]))
            continue;
        // Walk both polygons from here, wrapping around the first one.
        const UnkPolyPoint_l118ef0* p = start;
        const UnkPolyPoint_l118ef0* q = other->begin();
        for (u64 i = 0; i < other->count; ++i) {
            if (!NearlyEqual(*p, *q))
                break;
            ++q;
            p = p + 1 == self->end() ? self->begin() : p + 1;
        }
        if (q == other->end())
            return true;
    }
    return false;
}
