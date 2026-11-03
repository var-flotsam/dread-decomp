// CSpotLightComponent unit (tools/tu_map.py, the name from reflection): a reflected getter returning the
// length of the vector at +0x30 of the light reached through the owner's +0x58 object (+0x138, +0x78)
// as a float value. Names are placeholders.
#include <math.h>

#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

UnkReflValue fn_71008f4c34(void*, char* obj) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    char* a = *reinterpret_cast<char**>(target + 0x58);
    char* b = *reinterpret_cast<char**>(a + 0x138);
    const f32* v = reinterpret_cast<const f32*>(*reinterpret_cast<char**>(b + 0x78) + 0x30);
    f32 value = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    const base::reflection::CType* type = fn_710000abbc();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value, false);
    return r;
}
