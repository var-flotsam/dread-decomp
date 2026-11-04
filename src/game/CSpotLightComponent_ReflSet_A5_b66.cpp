// CSpotLightComponent's unit: a reflected setter for a float of the light reached through the owner's
// +0x58 object (+0x138, +0x78; the value at +0x110), kept at 0.01 or more. Answers the empty status
// {0, 0, 2, 1} (CEmmyValveComponent_ReflInvoker_A5_b65.cpp). Names are placeholders.
#include <math.h>

#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

static inline __attribute__((always_inline)) void* ArgPtr_A5_b66(UnkReflValue* arg) {
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_71008fa600(void*, char* obj, UnkReflValue* const* args) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    f32 value = *static_cast<const f32*>(ArgPtr_A5_b66(&args[0][0]));
    char* a = *reinterpret_cast<char**>(target + 0x58);
    char* b = *reinterpret_cast<char**>(a + 0x138);
    char* light = *reinterpret_cast<char**>(b + 0x78);
    *reinterpret_cast<f32*>(light + 0x110) = value < 0.01f ? 0.01f : value;
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}
