// CCaterzillaAIComponent (tools/tu_map.py): after the base routine (fn_7101111c24) fills `out`, scales
// its two vectors (+0x60, +0x6c) by the component's factor (+0x14728) while +0x14724 is set. The
// factor is read again for the second vector (a float store could change it). Names and layouts are
// placeholders.
#include "types.h"

struct UnkCaterzillaOut_b49 {
    u8 pad_0x0[0x60];
    f32 a[3];
    f32 b[3];
};

struct UnkCaterzillaAIScale_b49 {
    u8 pad_0x0[0x14724];
    bool scaled;
    u8 pad_0x14725[3];
    f32 factor;
};

void fn_7101111c24(UnkCaterzillaAIScale_b49* self, UnkCaterzillaOut_b49* out);

void fn_71003f8450(UnkCaterzillaAIScale_b49* self, UnkCaterzillaOut_b49* out) {
    fn_7101111c24(self, out);
    if (self->scaled) {
        f32 f = self->factor;
        out->a[0] = f * out->a[0];
        out->a[1] = f * out->a[1];
        out->a[2] = f * out->a[2];
        f = self->factor;
        out->b[0] = f * out->b[0];
        out->b[1] = f * out->b[1];
        out->b[2] = f * out->b[2];
    }
}
