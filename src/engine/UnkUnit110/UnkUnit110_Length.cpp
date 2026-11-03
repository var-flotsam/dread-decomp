// tools/tu_map.py unit 110: the length of the 2D vector held at +0x10. Names are placeholders.
#include <math.h>

#include "types.h"

struct UnkVectorHolder110 {
    u8 pad_0x0[0x10];
    f32 x;  // 0x10
    f32 y;  // 0x14
};

f32 fn_7100125a3c(const UnkVectorHolder110* self) {
    return sqrtf(self->x * self->x + self->y * self->y);
}
