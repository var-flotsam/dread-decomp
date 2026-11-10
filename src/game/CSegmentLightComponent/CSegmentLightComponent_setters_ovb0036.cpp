// CSegmentLightComponent's unit (tools/tu_map.py): reflected colour setters of the light reached through the
// owner's +0x58 object (+0x138, +0x78), each component kept at 0 or more, and a setter forwarding a float
// argument to the +0x20 block of the +0x58 object. Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "base/math/CVector4D.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::color::CColor4F;
using base::math::CVector4D;
using base::reflection::UnkReflValue;
using base::reflection::refl_ArgValue;
using base::reflection::refl_EmptyValue;

void fn_710020c9ac(void* block, f32 value);

static inline __attribute__((always_inline)) f32 NonNegative_ovb0036(f32 v) {
    return v <= 0.0f ? 0.0f : v;
}

// Each component of `color` clamped to be non-negative, assigned to the colour at `offset` of the light.
static inline __attribute__((always_inline)) void SetLightColor_ovb0036(char* self, const CColor4F* color,
                                                                        u32 offset) {
    char* a = *reinterpret_cast<char**>(self + 0x58);
    char* b = *reinterpret_cast<char**>(a + 0x138);
    char* light = *reinterpret_cast<char**>(b + 0x78);
    CColor4F c(NonNegative_ovb0036(color->r), NonNegative_ovb0036(color->g), NonNegative_ovb0036(color->b),
               NonNegative_ovb0036(color->a));
    fn_710007b518(reinterpret_cast<CVector4D*>(light + offset), reinterpret_cast<const CVector4D*>(&c));
}

void fn_71008f44f8(char* self, const CColor4F* color) {
    SetLightColor_ovb0036(self, color, 0x4c);
}

void fn_71008f4570(char* self, const CColor4F* color) {
    SetLightColor_ovb0036(self, color, 0x3c);
}

void fn_71008f45e8(char* self, const CColor4F* color) {
    SetLightColor_ovb0036(self, color, 0x5c);
}

void fn_71008f4660(char* self, const CColor4F* color) {
    SetLightColor_ovb0036(self, color, 0x6c);
}

UnkReflValue fn_71008f4ed0(void*, char* obj, UnkReflValue* const* args) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    f32 value = *refl_ArgValue<f32>(args[0]);
    fn_710020c9ac(*reinterpret_cast<char**>(target + 0x58) + 0x20, value);
    return refl_EmptyValue();
}
