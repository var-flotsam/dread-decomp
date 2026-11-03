// From unit 307 of tools/tu_map.py (0x7100215dac-0x710021685c): three (min, max) float ranges from
// +8; shifts each valid range (min <= max) by its own offset. Names and layouts are placeholders.
#include "types.h"

struct UnkUnit307FloatRange {
    float min;
    float max;
};

struct UnkUnit307Ranges {
    u8 pad_0x0[0x8];
    UnkUnit307FloatRange ranges[3];  // 0x8
};

void fn_7100216758(UnkUnit307Ranges* self, const float* offsets) {
    for (int i = 0; i < 3; ++i) {
        UnkUnit307FloatRange& r = self->ranges[i];
        float o = offsets[i];
        float min = r.min;
        float max = r.max;
        if (min <= max) {
            r.max = max + o;
            r.min = min + o;
        }
    }
}
