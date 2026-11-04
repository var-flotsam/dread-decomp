// CSegmentLightComponent's unit: a reflected getter returning the unit direction of the vector at
// +0x30 of the light reached through the owner's +0x58 object (+0x138, +0x78), as a 3D vector value
// (divided by its length plus 0.0001). As fn_71008f4c34 (CSpotLightComponent_Length_b50.cpp).
// Names are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::math::CVector3D;
using base::reflection::UnkReflValue;

UnkReflValue fn_71008f49e4(void*, char* obj) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    char* a = *reinterpret_cast<char**>(target + 0x58);
    char* b = *reinterpret_cast<char**>(a + 0x138);
    const f32* v = reinterpret_cast<const f32*>(*reinterpret_cast<char**>(b + 0x78) + 0x30);
    f32 inv = 1.0f / (sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]) + 0.0001f);
    f32 x = v[0] * inv;
    f32 y = v[1] * inv;
    f32 z = v[2] * inv;
    CVector3D value;
    value.x = x;
    value.y = y;
    value.z = z;
    const base::reflection::CType* type = fn_71000246e4();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value, false);
    return r;
}
