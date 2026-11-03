// Functions in the CAutofocusAimCameraCtrl unit (tools/tu_map.py). Names and layouts are
// placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;
using base::reflection::refl_ArgValue;
using base::reflection::refl_EmptyValue;

// A property setter: writes the f32 argument at +0x70, clamped to [0, 90] (NaN passes the lower bound).
UnkReflValue fn_7100723730(void*, char* obj, UnkReflValue* const* args) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    f32 v = *refl_ArgValue<f32>(args[0]);
    f32 clamped = v >= 90.0f ? 90.0f : v;
    f32 r = 0.0f;
    if (!(v <= 0.0f))
        r = clamped;
    *reinterpret_cast<f32*>(target + 0x70) = r;
    return refl_EmptyValue();
}
