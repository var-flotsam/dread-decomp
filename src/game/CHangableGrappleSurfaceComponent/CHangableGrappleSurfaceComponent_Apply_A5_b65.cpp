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

void CHangableGrappleSurfaceComponent_FadeMaterialAlpha(UnkGrappleSurface_A5_b65* self, f32 factor);

void CHangableGrappleSurfaceComponent_FadeGlowToFullOrNotAllowedIntensity(UnkGrappleSurface_A5_b65* self, bool forced) {
    if (forced)
        CHangableGrappleSurfaceComponent_FadeMaterialAlpha(self, 1.0f);
    else
        CHangableGrappleSurfaceComponent_FadeMaterialAlpha(self, self->tunable->factor);
}
