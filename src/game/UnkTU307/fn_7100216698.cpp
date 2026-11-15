// Unit 307: three (min, max) float ranges from +8; each valid range (min <= max) is scaled about its own
// midpoint by a per-axis factor. Names and layouts are placeholders.
#include "types.h"

struct UnkRange {
    f32 min;
    f32 max;
};

struct UnkRanges {
    u8 pad_0x0[0x8];
    UnkRange ranges[3];  // 0x8
};

static inline f32 Midpoint(const UnkRange& r) { return (r.min + r.max) * 0.5f; }

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_7100216698(UnkRanges* self, const f32* scale) {
    for (int i = 0; i < 3; ++i) {
        UnkRange& r = self->ranges[i];
        if (r.min <= r.max) {
            f32 mid = Midpoint(r);
            f32 s = scale[i];
            f32 newMax = mid + s * (r.max - mid);
            f32 newMin = mid + s * (r.min - mid);
            r.max = newMax;
            r.min = newMin;
        }
    }
}
