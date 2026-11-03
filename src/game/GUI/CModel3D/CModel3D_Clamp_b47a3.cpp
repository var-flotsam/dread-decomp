// GUI::CModel3D unit (tools/tu_map.py): stores a value at +0x228, clamped to [+0x22c, +0x230] when asked.
// Names and layouts are placeholders.
#include "types.h"

struct UnkModel3DRange_b47a3 {
    u8 unk_0x0[0x228];
    f32 value;
    f32 min;
    f32 max;
};

void fn_71011f48b0(UnkModel3DRange_b47a3* self, bool clamp, f32 value) {
    if (clamp) {
        f32 r;
        if (self->min >= value)
            r = self->min;
        else
            r = self->max <= value ? self->max : value;
        self->value = r;
    } else {
        self->value = value;
    }
}
