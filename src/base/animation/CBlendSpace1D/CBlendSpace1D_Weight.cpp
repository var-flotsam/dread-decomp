// base::animation::CBlendSpace1D's unit (unit 13): the blend's total, recomputed when its dirty
// flag (bit 1 of +0x40) is set: the sum over its two inputs of each one's sample (its source's
// vf_0x30 at the input's weight) over the source's length, times the weight. Names and layouts
// are placeholders.
#include "base/animation/CBlendSpace1D.h"
#include "types.h"

struct UnkBlendSource13 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual f32 vf_0x30(f32 at);
};

struct UnkBlendTrack13 {
    u32 field_0x0;
    f32 length;
    UnkBlendSource13* source;
};

struct UnkBlendInput13 {
    f32 weight;
    UnkBlendTrack13* track;
};

struct UnkBlend13 {
    u8 pad_0x0[0x8];
    UnkBlendInput13 inputs[2];  // 0x8
    u8 pad_0x28[0x10];
    f32 total;  // 0x38
    u32 pad_0x3c;
    u32 flags;  // 0x40
};
static_assert(__builtin_offsetof(UnkBlend13, flags) == 0x40, "UnkBlend13 layout");

static inline __attribute__((always_inline)) void AddInput(f32& total, UnkBlendInput13& input) {
    if (input.weight > 0.0f && input.track) {
        f32 length = input.track->length;
        f32 value = length != 0.0f ? input.track->source->vf_0x30(input.weight) / length * input.weight : 0.0f;
        total += value;
    }
}

f32 fn_71000070bc(UnkBlend13* self) {
    fn_7100006e98(self, 0);
    if (!(self->flags & 2))
        return self->total;
    f32 total = 0.0f;
    self->flags &= ~2u;
    AddInput(total, self->inputs[0]);
    AddInput(total, self->inputs[1]);
    self->total = total;
    return total;
}
