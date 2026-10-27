// CPickable's unit (its reflection is src/reflection/CPickable.cpp): fn_7100f061a8 sets the
// alpha of the parts a pickable draws with. Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "types.h"

using base::color::CColor4F;

// A drawn part: its colour at +0x118.
struct UnkPickablePart {
    u8 pad_0x0[0x118];
    CColor4F color;
    u8 pad_0x128[4];
    u32 field_0x12c;
};

// The parts (up to two, from +0x20) and the alpha last set (+0xa0).
struct UnkPickableParts {
    u8 pad_0x0[0x20];
    UnkPickablePart* parts[2];
    u32 count;
    u8 pad_0x34[0x6c];
    f32 alpha;
};
static_assert(__builtin_offsetof(UnkPickableParts, alpha) == 0xa0, "UnkPickableParts layout");

// Stores the alpha and gives every part its colour with that alpha (not below 0).
void fn_7100f061a8(UnkPickableParts* self, f32 alpha) {
    self->alpha = alpha;
    for (u64 i = 0; i < self->count; ++i) {
        UnkPickablePart* part = self->parts[i];
        CColor4F color;
        color.r = part->color.r;
        color.g = part->color.g;
        color.b = part->color.b;
        color.a = alpha <= 0.0f ? 0.0f : alpha;
        fn_7100030728(&part->color, &color);
        part->field_0x12c = 0;
    }
}
