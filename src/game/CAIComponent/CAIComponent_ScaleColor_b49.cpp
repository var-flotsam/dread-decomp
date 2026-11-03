// CAIComponent unit (tools/tu_map.py unit 2702, medium confidence): fn_7101111c24 scales two groups of
// three floats (+0x60 and +0x6c of `out`) by the factor at +0x14504 when the flag at +0x41a of the object
// behind +0x1610 is set and the factor is positive. Names and layouts are placeholders.
#include "types.h"

struct UnkAIScaleFlags_b49 {
    u8 pad_0x0[0x41a];
    bool enabled;
};

struct UnkAIScaleSource_b49 {
    u8 pad_0x0[0x1610];
    UnkAIScaleFlags_b49* flags;
    u8 pad_0x1618[0x12eec];
    f32 factor;  // 0x14504
};

struct UnkAIScaleTarget_b49 {
    u8 pad_0x0[0x60];
    f32 a[3];
    f32 b[3];
};

void fn_7101111c24(UnkAIScaleSource_b49* self, UnkAIScaleTarget_b49* out) {
    if (!self->flags->enabled)
        return;
    f32 factor = self->factor;
    if (factor > 0.0f) {
        out->a[0] = factor * out->a[0];
        out->a[1] = factor * out->a[1];
        out->a[2] = factor * out->a[2];
        factor = self->factor;
        out->b[0] = factor * out->b[0];
        out->b[1] = factor * out->b[1];
        out->b[2] = factor * out->b[2];
    }
}
