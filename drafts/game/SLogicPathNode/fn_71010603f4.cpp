// SLogicPathNode unit: equality of two arrays of 2D vectors, each component within a global tolerance.
// Names and layouts are placeholders.
#include "types.h"

extern const f32 g_unk_7101647774;

struct UnkApproxVec2f {
    f32 x, y;

    bool operator==(const UnkApproxVec2f& o) const {
        return x - g_unk_7101647774 <= o.x && g_unk_7101647774 + x >= o.x && y - g_unk_7101647774 <= o.y &&
               y + g_unk_7101647774 >= o.y;
    }
};

struct UnkVec2fArray {
    UnkApproxVec2f* data;
    u32 count;  // 0x8
};

bool fn_71010603f4(const UnkVec2fArray* a, const UnkVec2fArray* b) {
    u32 n = a->count;
    if (n != b->count)
        return false;
    const UnkApproxVec2f* pb = b->data;
    const UnkApproxVec2f* pa = a->data;
    for (u32 i = 0; i < n; ++i) {
        if (!(pb[i] == pa[i]))
            return false;
    }
    return true;
}
