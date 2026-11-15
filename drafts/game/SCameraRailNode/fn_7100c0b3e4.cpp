// Unit SCameraRailNode: compares two arrays of 2-float points, equal when the counts match and every
// component pair lies within a tolerance. Names and layouts are placeholders.
#include "types.h"

extern f32 g_unk_7101647774;

struct UnkPoint2 {
    f32 x;
    f32 y;
};

struct UnkPointArray {
    UnkPoint2* data;
    u32 count;  // 0x8
};

static inline bool Near(f32 a, f32 b) {
    f32 eps = g_unk_7101647774;
    return b - eps <= a && b + eps >= a;
}

bool fn_7100c0b3e4(const UnkPointArray* a, const UnkPointArray* b) {
    u32 n = a->count;
    if (n != b->count)
        return false;
    for (u32 i = 0; i < n; ++i) {
        if (!Near(a->data[i].x, b->data[i].x) || !Near(a->data[i].y, b->data[i].y))
            return false;
    }
    return true;
}
