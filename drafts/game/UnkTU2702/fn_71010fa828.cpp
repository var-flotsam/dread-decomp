#include "types.h"

extern "C" int rand();

struct UnkAIRangeSrc_b0144 {
    void** vtable;
    u8 pad_0x8[0x12c];
    f32 field_0x134;
    void vf_0x90(void* owner, f32* low, f32* high) {
        reinterpret_cast<void (*)(UnkAIRangeSrc_b0144*, void*, f32*, f32*)>(vtable[0x90 / 8])(this, owner, low, high);
    }
};

struct UnkAIK_b0144 {
    u8 pad_0x0[0x30];
    void* owner;  // 0x30
    u8 pad_0x38[0x1610 - 0x38];
    UnkAIRangeSrc_b0144* source;  // 0x1610
    u8 pad_0x1618[0x1468c - 0x1618];
    f32 value;     // 0x1468c
    f32 min_edge;  // 0x14690
    f32 max_edge;  // 0x14694
};
static_assert(__builtin_offsetof(UnkAIK_b0144, value) == 0x1468c, "layout");

void fn_71010fa828(UnkAIK_b0144* self) {
    f32* value = &self->value;
    f32 low_value = -1.0f;
    f32 high_value = -1.0f;
    f32* low = &low_value;
    f32* high = &high_value;
    self->source->vf_0x90(self->owner, low, high);
    f32 min_edge = *reinterpret_cast<f32*>(reinterpret_cast<u8*>(self) + 0x14690);
    if (min_edge > -1.0f) {
        f32 max_edge = value[2];
        if (max_edge > -1.0f) {
            *low = __builtin_fmaxf(*low, min_edge);
            *high = __builtin_fminf(*high, max_edge);
        }
    }
    f32 current = value[0];
    f32 hi = *high;
    f32 lo = *low;
    if (current >= lo && hi >= current) {
        f32 step = self->source->field_0x134;
        f32 to_lo = current - lo;
        f32 to_hi = current - hi;
        to_lo = __builtin_fabsf(to_lo);
        to_hi = __builtin_fabsf(to_hi);
        if (to_lo < to_hi) {
            f32 x = current + step;
            lo = __builtin_fminf(x, hi);
            *low = lo;
        } else {
            f32 x = current - step;
            hi = __builtin_fmaxf(lo, x);
            *high = hi;
        }
    }
    f32 t = static_cast<f32>(static_cast<f64>(rand() & 0x7fff) * 0x1.0002000400080p-15);
    value[0] = lo + (hi - lo) * t;
}
