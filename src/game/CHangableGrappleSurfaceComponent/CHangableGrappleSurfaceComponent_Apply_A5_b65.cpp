// CHangableGrappleSurfaceComponent's unit: applies a factor (fn_710082566c), 1 when forced, else the
// tunable's value at +0x68 of the object at +0x98. Names are placeholders.
#include "types.h"

struct UnkGrappleSurfaceTunable_A5_b65 {
    u8 pad_0x0[0x68];
    f32 factor;
};

struct UnkGrappleSurface_A5_b65 {
    u8 pad_0x0[0x98];
    UnkGrappleSurfaceTunable_A5_b65* tunable;
};

void fn_710082566c(UnkGrappleSurface_A5_b65* self, f32 factor);

void fn_7100825654(UnkGrappleSurface_A5_b65* self, bool forced) {
    if (forced)
        fn_710082566c(self, 1.0f);
    else
        fn_710082566c(self, self->tunable->factor);
}
