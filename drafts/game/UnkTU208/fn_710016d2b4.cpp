// Unit 208: samples a keyframed curve at a scaled time and writes the value into the owner's output array.
// Names and layouts are placeholders.
#include "types.h"

struct UnkCurveKey {
    f32 time;
    f32 in_slope;
    f32 out_slope;
    f32 value;
};

struct UnkCurveSource {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual f32 vf_0x10(void* arg, u32 index);
};

struct UnkCurveOwner {
    u8 pad_0x00[8];
    void* field_0x08;
    u8 pad_0x10[8];
    f32* outputs;  // 0x18
};

struct UnkCurveNode {
    u8 pad_0x00[8];
    UnkCurveOwner* owner;
    UnkCurveSource* source;
    f32 scale;
    u8 pad_0x1c[4];
    f32 value;  // 0x20
    u8 pad_0x24[4];
    u32 key_count;  // 0x28
    u8 pad_0x2c[8];
    f32 default_value;  // 0x34
    UnkCurveKey* keys;  // 0x38
};

void fn_710016d2b4(UnkCurveNode* self, u32 index) {
    UnkCurveOwner* owner = self->owner;
    f32 t = self->source->vf_0x10(*(void**)((u8*)owner + 8), index);
    f32 result;
    if (self->key_count == 0 || !self->keys) {
        result = self->default_value;
    } else {
        UnkCurveKey* keys = self->keys;
        t = t * self->scale;
        u32 last = self->key_count - 1;
        if (keys[0].time >= t) {
            result = keys[0].value;
        } else if (keys[last].time <= t) {
            result = keys[last].value;
        } else {
            UnkCurveKey* hi = &keys[last];
            UnkCurveKey* lo = keys;
            u32 a = 0;
            u32 b = last;
            while (a != b) {
                u32 mid = (b + a) >> 1;
                UnkCurveKey* k = &keys[mid];
                if (!(k->time > t)) {
                    a = mid + 1;
                    lo = k;
                } else {
                    hi = k;
                    b = mid;
                }
            }
            f32 dt = t - lo->time;
            f32 span = hi->time - lo->time;
            f32 u = dt / span;
            f32 um1 = u + -1.0f;
            f32 blend = (lo->value - hi->value) * (u + u + -3.0f);
            f32 smooth = lo->value + u * (u * blend);
            f32 slope = um1 * lo->out_slope + u * hi->in_slope;
            result = smooth + dt * um1 * slope;
        }
    }
    self->value = result;
    self->owner->outputs[index] = result;
}
