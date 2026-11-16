// base::animation::CBlendSpace1D's unit (unit 13): when the blend is dirty (bit 0 of +0x40) or the
// update is forced, picks the two samples around the blend position, sets their weights, and advances
// each sample with a positive weight by the step. Names and layouts are placeholders.
#include "types.h"

struct UnkBlend13;

struct UnkBlendSource_l006e98 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50(f32 step);
};

struct UnkBlendSample_l006e98 {
    f32 position;
    u32 pad_0x4;
    UnkBlendSource_l006e98* source;  // 0x8
    u8 pad_0x10[0x8];
};
static_assert(sizeof(UnkBlendSample_l006e98) == 0x18, "layout");

struct UnkBlendInput_l006e98 {
    f32 weight;
    UnkBlendSample_l006e98* sample;

    void Set(f32 w, UnkBlendSample_l006e98* s) {
        weight = w;
        sample = s;
    }
};

struct UnkBlendSpace1D_l006e98 {
    u8 pad_0x0[0x8];
    UnkBlendInput_l006e98 inputs[2];  // 0x8
    u8 pad_0x28[0x8];
    f32 position;  // 0x30
    f32 step;      // 0x34
    u8 pad_0x38[0x8];
    u32 flags;  // 0x40
    u32 pad_0x44;
    UnkBlendSample_l006e98* samples;  // 0x48
    u32 count;                        // 0x50

    // Advances the input's sample by the step when the input has weight.
    void Advance(UnkBlendInput_l006e98& input) {
        if (input.weight > 0.0f && input.sample)
            input.sample->source->vf_0x50(step);
    }
};
static_assert(__builtin_offsetof(UnkBlendSpace1D_l006e98, count) == 0x50, "layout");

bool fn_7100006e98(UnkBlend13* blend, bool force) {
    UnkBlendSpace1D_l006e98* self = reinterpret_cast<UnkBlendSpace1D_l006e98*>(blend);
    if (!(self->flags & 1) && !force)
        return false;
    u32 count = self->count;
    self->flags &= ~1u;
    switch (count) {
    case 0:
        break;
    case 1:
        self->inputs[0].Set(1.0f, &self->samples[0]);
        self->inputs[1].Set(0.0f, nullptr);
        break;
    default: {
        UnkBlendSample_l006e98* first = &self->samples[0];
        f32 x = self->position;
        if (first->position >= x) {
            self->inputs[0].Set(1.0f, first);
            self->inputs[1].Set(0.0f, nullptr);
            break;
        }
        u64 last_index = static_cast<u64>(count) - 1;
        UnkBlendSample_l006e98* last = &self->samples[last_index];
        if (last->position <= x) {
            self->inputs[0].Set(1.0f, last);
            self->inputs[1].Set(0.0f, nullptr);
            break;
        }
        u64 i = 1;
        while (i != last_index && self->samples[i].position <= x)
            ++i;
        UnkBlendSample_l006e98* upper = &self->samples[i];
        UnkBlendSample_l006e98* lower = &self->samples[i - 1];
        f32 t = (x - lower->position) / (upper->position - lower->position);
        self->inputs[0].Set(1.0f - t, lower);
        self->inputs[1].Set(t, upper);
        break;
    }
    }
    self->Advance(self->inputs[0]);
    self->Advance(self->inputs[1]);
    return true;
}
