// CCharClassChozoCommanderSentenceSphereMovementComponent's unit (tools/tu_map.py unit 669): a
// reflected property setter (invoker of fn_71004590e4's function): stores the float argument, in
// degrees, at +0x134 of the object at +8 and its value in radians at +0x138. Names and layouts are
// placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;
using base::reflection::refl_ArgValue;
using base::reflection::refl_EmptyValue;

UnkReflValue fn_71004593bc(void*, char* obj, UnkReflValue* const* args) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    f32 degrees = *refl_ArgValue<f32>(args[0]);
    *reinterpret_cast<f32*>(target + 0x134) = degrees;
    *reinterpret_cast<f32*>(target + 0x138) = (f32)(degrees * 3.14159265358979323846 / 180.0);
    return refl_EmptyValue();
}
