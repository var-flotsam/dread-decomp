// CDirLightComponent's unit (tools/tu_map.py): reflected colour setters of the light reached through the
// owner's +0x58 object (+0x138, +0x78), each component kept at 0 or more; setters forwarding a float or a
// flag argument to the +0x20 block of the +0x58 object; and fn_71008eb348, which marks that block (fn_710020c8ac)
// and, when the angles of the transform of the object at +0x30 moved by more than 0.001 from the ones cached at
// +0x4c, caches them and points the light's direction (+0x30; dirty flag +0x1b8, then fn_71001bdc3c) along x
// rotated by them. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/color/CColor4F.h"
#include "base/math/CVector4D.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU251.h"
#include "game/UnkTU290.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::color::CColor4F;
using base::math::CVector4D;
using base::reflection::UnkReflValue;
using base::reflection::refl_ArgValue;
using base::reflection::refl_EmptyValue;

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

void fn_71008eb574(char* self, const CColor4F* color) {
    SetLightColor_ovb0036(self, color, 0x4c);
}

void fn_71008eb5ec(char* self, const CColor4F* color) {
    SetLightColor_ovb0036(self, color, 0x3c);
}

void fn_71008eb664(char* self, const CColor4F* color) {
    SetLightColor_ovb0036(self, color, 0x5c);
}

void fn_71008eb6dc(char* self, const CColor4F* color) {
    SetLightColor_ovb0036(self, color, 0x6c);
}

UnkReflValue fn_71008ebb88(void*, char* obj, UnkReflValue* const* args) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    f32 value = *refl_ArgValue<f32>(args[0]);
    fn_710020c740(*reinterpret_cast<char**>(target + 0x58) + 0x20, value);
    return refl_EmptyValue();
}

UnkReflValue fn_71008ebc44(void*, char* obj, UnkReflValue* const* args) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    bool value = *refl_ArgValue<bool>(args[0]);
    fn_710020c494(*reinterpret_cast<char**>(target + 0x58) + 0x20, value);
    return refl_EmptyValue();
}

struct UnkMat4f;

UnkVec3f fn_7100078170(const UnkMat4f* m, const UnkVec3f* v);

// Whether `a` is within 0.001 of `b`.
static inline __attribute__((always_inline)) bool Near_ovb0036(f32 a, f32 b) {
    return a - 0.001f <= b && a + 0.001f >= b;
}

// `v` through the matrix `m` (fn_7100078170).
static inline __attribute__((always_inline)) UnkVec3f Rotate_ovb0036(const UnkMatrix44& m, const UnkVec3f& v) {
    return fn_7100078170(reinterpret_cast<const UnkMat4f*>(&m), &v);
}

struct UnkDirLightOwner_ovb0036 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkDirLightComponent_ovb0036 {
    u8 pad_0x0[0x30];
    UnkDirLightOwner_ovb0036* owner;  // 0x30
    u8 pad_0x38[0x14];
    f32 angle[3];  // 0x4c
    char* light;   // 0x58
};
static_assert(__builtin_offsetof(UnkDirLightComponent_ovb0036, light) == 0x58, "layout");

void fn_71008eb348(UnkDirLightComponent_ovb0036* self) {
    fn_710020c8ac(self->light + 0x20, true);
    UnkVec3f angle = self->owner->xform.GetAngle();
    if (!Near_ovb0036(angle.x, self->angle[0]) || !Near_ovb0036(angle.y, self->angle[1]) ||
        !Near_ovb0036(angle.z, self->angle[2])) {
        self->angle[0] = angle.x;
        self->angle[1] = angle.y;
        self->angle[2] = angle.z;
        UnkVec3f direction =
            Rotate_ovb0036(fn_7100078fc8(reinterpret_cast<const UnkMatrix44&>(angle)), UnkVec3f(1.0f, 0.0f, 0.0f));
        char* b = *reinterpret_cast<char**>(self->light + 0x138);
        char* light = *reinterpret_cast<char**>(b + 0x78);
        *reinterpret_cast<UnkVec3f*>(light + 0x30) = direction;
        *reinterpret_cast<u8*>(light + 0x1b8) = 1;
        fn_71001bdc3c(b);
    }
}
