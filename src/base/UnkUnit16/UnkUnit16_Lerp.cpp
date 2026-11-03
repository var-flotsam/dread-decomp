// tools/tu_map.py unit 16 (0x7100011824-0x7100011a60): a linear blend of two floats by a factor,
// all three passed by pointer; the first argument is unused. Names are placeholders.
#include "types.h"

static inline f32 Lerp16(f32 a, f32 b, f32 t) {
    f32 inv = 1.0f - t;
    return b * t + a * inv;
}

f32 fn_710001190c(void* context, const f32* from, const f32* to, const f32* t) {
    return Lerp16(*from, *to, *t);
}
