// Unit 307: three (min, max) float ranges from +8; each valid range (min <= max) is scaled about a per-axis
// centre. Names and layouts are placeholders.
#include "game/UnkRange.h"
#include "game/UnkRanges.h"
#include "types.h"

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
void fn_71002165e8(UnkRanges* self, const f32* scale, const f32* centre) {
    for (int i = 0; i < 3; ++i) {
        UnkRange& r = self->ranges[i];
        f32 min = r.min;
        f32 max = r.max;
        if (min <= max) {
            f32 c = centre[i];
            f32 s = scale[i];
            f32 newMax = c + s * (max - c);
            f32 newMin = c + s * (min - c);
            r.max = newMax;
            r.min = newMin;
        }
    }
}
