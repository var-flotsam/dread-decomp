// nn::atk::detail::BiquadFilterLpf vtable slot 0x10: picks one of 112 precomputed
// coefficient sets from a 0..1 value.
#include "types.h"

struct UnkBiquadCoef {
    s16 b0, b1, b2, a1, a2;
};

extern const UnkBiquadCoef g_unk_7101d40790[112];

// NON_MATCHING(minor): the 10-byte struct copy is emitted low word first; the original
// copies the trailing halfword first. No tested compiler reproduces the original order.
void fn_7101215464(const void* self, f32 value, UnkBiquadCoef* out) {
    s32 index = static_cast<s32>(value * 111.0f);
    if (index < 0)
        index = 0;
    if (index > 111)
        index = 111;
    *out = g_unk_7101d40790[index];
}
