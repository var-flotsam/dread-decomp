// Unit 2566 (minimap manager): equality of two records with the same owner and a position within a tolerance.
// Names and layouts are placeholders.
#include "types.h"

extern const f32 g_unk_7101647774;

struct UnkApproxPos {
    f32 x, y;

    bool operator==(const UnkApproxPos& o) const {
        return x - g_unk_7101647774 <= o.x && x + g_unk_7101647774 >= o.x && y - g_unk_7101647774 <= o.y &&
               y + g_unk_7101647774 >= o.y;
    }
};

struct UnkMapPoint {
    u64 owner;
    UnkApproxPos pos;
};

bool fn_7100eacd7c(UnkMapPoint* a, UnkMapPoint* b) {
    if (a->owner == b->owner) {
        return a->pos == b->pos;
    }
    return false;
}
