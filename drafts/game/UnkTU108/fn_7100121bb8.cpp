// Unit 108: compares two arrays of float pairs; equal when the counts match and every component of
// the first lies within a global tolerance of the second's. Names and layouts are placeholders.
#include "types.h"

extern const f32 g_unk_7101647774;

struct UnkVec2_108 {
    f32 x;
    f32 y;
};

struct UnkVec2Array108 {
    UnkVec2_108* data;
    u32 count;  // 0x8
};

static inline bool Near(f32 center, f32 value) {
    f32 lo = center - g_unk_7101647774;
    f32 hi = g_unk_7101647774 + center;
    return hi >= value && lo <= value;
}

bool fn_7100121bb8(const UnkVec2Array108* a, const UnkVec2Array108* b) {
    u64 n = a->count;
    if (a->count != b->count)
        return false;
    if (n) {
        const UnkVec2_108* pa = a->data;
        const UnkVec2_108* pb = b->data;
        do {
            if (!Near(pb->x, pa->x))
                return false;
            if (!Near(pb->y, pa->y))
                return false;
            ++pa;
            ++pb;
        } while (--n);
    }
    return true;
}
