// tools/tu_map.py unit 300 (no class hint): whether two integer boxes (min at 0x20, max at 0x2c)
// overlap, bounds included. Names and layouts are placeholders.
#include "types.h"

struct UnkIntBox300_b66a1 {
    u8 pad_0x0[0x20];
    s32 min[3];  // 0x20
    s32 max[3];  // 0x2c
};

bool fn_7100211bd8(const UnkIntBox300_b66a1* self, const UnkIntBox300_b66a1* other) {
    for (s32 i = 0; i < 3; ++i) {
        if (other->max[i] < self->min[i] || other->min[i] > self->max[i]) {
            return false;
        }
    }
    return true;
}
