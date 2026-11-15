// Unit 307: three (min, max) float ranges from +8; each valid range (min <= max) has its min and max moved by
// separate per-axis amounts. Names and layouts are placeholders.
#include "types.h"

struct UnkRange {
    f32 min;
    f32 max;
};

struct UnkRanges {
    u8 pad_0x0[0x8];
    UnkRange ranges[3];  // 0x8
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_7100216568(UnkRanges* self, const f32* minDelta, const f32* maxDelta) {
    for (int i = 0; i < 3; ++i) {
        UnkRange& r = self->ranges[i];
        f32 min = r.min;
        f32 max = r.max;
        if (min <= max) {
            f32 newMin = min + minDelta[i];
            f32 newMax = max + maxDelta[i];
            r.max = newMax;
            r.min = newMin;
        }
    }
}
