// In the CCharClassDoorLifeComponent unit: two reflected setters of a colour (base::math::CVector4D),
// at +0x130 (fn_71008b309c) and +0x140 (fn_71008b3184) of the object, assigned through the vector's
// out-of-line assignment fn_710007b518. They return the empty value. Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::math::CVector4D;
using base::math::UnkVector4D;
using base::reflection::UnkReflValue;

namespace {
template <u32 Offset>
__attribute__((always_inline)) inline UnkReflValue SetVector(char* obj, UnkReflValue* const* args) {
    UnkReflValue* arg = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    UnkVector4D v(*base::reflection::refl_ArgValue<UnkVector4D>(arg));
    fn_710007b518(reinterpret_cast<CVector4D*>(target + Offset), &v);
    return base::reflection::refl_EmptyValue();
}
}  // namespace

UnkReflValue fn_71008b309c(void*, char* obj, UnkReflValue* const* args) {
    return SetVector<0x130>(obj, args);
}
UnkReflValue fn_71008b3184(void*, char* obj, UnkReflValue* const* args) {
    return SetVector<0x140>(obj, args);
}
