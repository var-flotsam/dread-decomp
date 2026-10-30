// CPickable's unit (its reflection is src/reflection/CPickable.cpp): fn_7100f061a8 sets the
// alpha of the parts a pickable draws with. Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "types.h"

using base::color::CColor4F;

// A drawn part: its colour at +0x118.
struct UnkPickablePart {
    u8 pad_0x0[0x24];
    bool field_0x24;
    bool field_0x25;
    u8 pad_0x26[0xf2];
    CColor4F color;
    u8 pad_0x128[4];
    u32 field_0x12c;
};

// The parts (up to two, from +0x20) and the alpha last set (+0xa0).
struct UnkPickableParts {
    u8 pad_0x0[0x20];
    UnkPickablePart* parts[2];
    union {
        u32 count;  // a union member: the original reloads it after each part's flag
    };
    u8 pad_0x34[0x41];
    union {
        bool field_0x75;  // reloaded after the parts' flags as well
    };
    bool field_0x76;
    u8 pad_0x77[0x29];
    f32 alpha;
    u8 pad_0xa4[0x28];
    bool field_0xcc;
    u8 pad_0xcd[0xf];
    bool field_0xdc;
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

// Sets the flag at +0x75 on the pickable and +0x24 on its parts; when it is set, the flag at +0x76 and
// +0x25 on the parts as well, else the parts are dropped, the flags at +0xcc and +0xdc cleared and
// +0x76 set to the (clear) flag.
void fn_7100f05fb4(UnkPickableParts* self, bool flag) {
    self->field_0x75 = flag;
    for (u64 i = 0; i < self->count; ++i)
        self->parts[i]->field_0x24 = flag;
    if (self->field_0x75) {
        self->field_0x76 = flag;
        for (u64 i = 0; i < self->count; ++i)
            self->parts[i]->field_0x25 = flag;
        return;
    }
    self->count = 0;
    self->field_0xcc = false;
    self->field_0xdc = false;
    self->field_0x76 = flag;
}
