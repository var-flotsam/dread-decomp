// In the CCharClassAIComponent unit: a reflected setter of the float at +0x23c of the object,
// clamped from below at 0 (a NaN is kept). It returns the empty value. Names are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

UnkReflValue fn_7100416508(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* arg = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    f32 value = *base::reflection::refl_ArgValue<f32>(arg);
    *reinterpret_cast<f32*>(target + 0x23c) = value < 0.0f ? 0.0f : value;
    return base::reflection::refl_EmptyValue();
}
