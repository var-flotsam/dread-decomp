// Unit 2566 (minimap manager): equality of two arrays of (key, 2D position) entries; positions match within a tolerance.
// Names and layouts are placeholders.
#include "types.h"

extern const f32 g_unk_7101647774;

struct UnkApproxPos {
    f32 x, y;

    bool operator==(const UnkApproxPos& o) const {
        return x - g_unk_7101647774 <= o.x && g_unk_7101647774 + x >= o.x && y - g_unk_7101647774 <= o.y &&
               g_unk_7101647774 + y >= o.y;
    }
};

struct UnkKeyedPos {
    u64 key;
    UnkApproxPos pos;
};

struct UnkKeyedPosArray {
    UnkKeyedPos* data;
    u32 count;
};

bool fn_7100eaf72c(const UnkKeyedPosArray* a, const UnkKeyedPosArray* b) {
    u32 n = a->count;
    if (n != b->count)
        return false;
    if (n) {
        const UnkKeyedPos* pa;
        const UnkKeyedPos* pb = b->data;
        pa = a->data;
        u64 left = n;
        do {
            if (!(pb->key == pa->key && pb->pos == pa->pos))
                return false;
            --left;
            ++pa;
            ++pb;
        } while (left);
    }
    return true;
}
