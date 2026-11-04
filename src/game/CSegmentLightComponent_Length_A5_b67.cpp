// CSegmentLightComponent's unit: a reflected setter for the length of the vector at +0x30 of the light reached
// through the owner's +0x58 object (+0x138, +0x78): the unit direction (divided by its length plus 0.0001, as the
// getter fn_71008f49e4 in CSegmentLightComponent_Direction_A5_b66.cpp) times the argument; it marks the light dirty
// (+0x1b8) and updates the +0x138 object (fn_71001bdc3c). Answers the empty status {0, 0, 2, 1}. Names are
// placeholders.
#include <math.h>

#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

void fn_71001bdc3c(void* object);

static inline __attribute__((always_inline)) void* ArgPtr_A5_b67(UnkReflValue* arg) {
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_71008f4cb8(void*, char* obj, UnkReflValue* const* args) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    f32 length = *static_cast<const f32*>(ArgPtr_A5_b67(&args[0][0]));
    char* a = *reinterpret_cast<char**>(target + 0x58);
    const f32* v = reinterpret_cast<const f32*>(*reinterpret_cast<char**>(*reinterpret_cast<char**>(a + 0x138) + 0x78) + 0x30);
    f32 inv = 1.0f / (sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]) + 0.0001f);
    f32 x = v[0] * inv;
    f32 y = v[1] * inv;
    f32 z = v[2] * inv;
    char* b = *reinterpret_cast<char**>(a + 0x138);
    char* light = *reinterpret_cast<char**>(b + 0x78);
    f32* dst = reinterpret_cast<f32*>(light + 0x30);
    dst[0] = length * x;
    dst[1] = length * y;
    dst[2] = length * z;
    *reinterpret_cast<u8*>(light + 0x1b8) = 1;
    fn_71001bdc3c(b);
    return base::reflection::refl_EmptyValue();
}
