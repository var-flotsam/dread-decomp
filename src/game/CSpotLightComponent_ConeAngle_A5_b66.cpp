// CSpotLightComponent's unit: a reflected setter for the cone angle (degrees, kept within (0, 180])
// of the light reached through the owner's +0x58 object (+0x138, +0x78): stores it at +0x84 and the
// cosine of its half (in radians) at +0x8c. Answers the empty status {0, 0, 2, 1}. Names are placeholders.
#include <math.h>

#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

static inline __attribute__((always_inline)) void* ArgPtr_A5_b66(UnkReflValue* arg) {
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_71008f9e20(void*, char* obj, UnkReflValue* const* args) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    f32 value = *static_cast<const f32*>(ArgPtr_A5_b66(&args[0][0]));
    char* a = *reinterpret_cast<char**>(target + 0x58);
    char* b = *reinterpret_cast<char**>(a + 0x138);
    char* light = *reinterpret_cast<char**>(b + 0x78);
    f32 angle = value <= 0.0f ? 0.0f : (value > 180.0f ? 180.0f : value);
    *reinterpret_cast<f32*>(light + 0x84) = angle;
    *reinterpret_cast<f32*>(light + 0x8c) = cosf((f32)((angle * 0.5f) * 3.141592653589793 / 180.0));
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}

// The same for the second cone (+0x88, cosine at +0x90), which also flags the light dirty (+0x1b8).
UnkReflValue fn_71008f9f50(void*, char* obj, UnkReflValue* const* args) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    f32 value = *static_cast<const f32*>(ArgPtr_A5_b66(&args[0][0]));
    char* a = *reinterpret_cast<char**>(target + 0x58);
    char* b = *reinterpret_cast<char**>(a + 0x138);
    char* light = *reinterpret_cast<char**>(b + 0x78);
    f32 angle = value <= 0.0f ? 0.0f : (value > 180.0f ? 180.0f : value);
    *reinterpret_cast<f32*>(light + 0x88) = angle;
    *reinterpret_cast<f32*>(light + 0x90) = cosf((f32)((angle * 0.5f) * 3.141592653589793 / 180.0));
    *reinterpret_cast<bool*>(light + 0x1b8) = true;
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}
