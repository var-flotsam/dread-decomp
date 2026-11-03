// Functions in the CBreakableTileGroupComponent unit (tools/tu_map.py). Names and layouts are
// placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;
using base::reflection::refl_ArgValue;
using base::reflection::refl_EmptyValue;

// The whole part of `v` (rounded toward zero), at least 0.
static inline f32 WholeNonNegative(f32 v) {
    f32 whole = v > 0.0f ? __builtin_floorf(v) : __builtin_ceilf(v);
    return __builtin_fmaxf(whole, 0.0f);
}

// A property setter: writes both components of the vector argument at +0x4 and +0x8, each cut to
// a whole number and raised to at least 0.
UnkReflValue fn_710075db78(void*, char* obj, UnkReflValue* const* args) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    const f32* v = refl_ArgValue<f32>(args[0]);
    *reinterpret_cast<f32*>(target + 0x4) = WholeNonNegative(v[0]);
    *reinterpret_cast<f32*>(target + 0x8) = WholeNonNegative(v[1]);
    return refl_EmptyValue();
}
