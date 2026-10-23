// A reflected property getter: the rotation angles (as a base::math::CVector3D) of the UnkXform
// at +0x38 in the object `obj` points to at +8, deriving and caching them on first use. The first
// parameter is unused.
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkXformNode.h"

using base::reflection::UnkReflValue;

UnkReflValue fn_710023e348(void*, char* obj) {
    UnkXform* x = reinterpret_cast<UnkXform*>(*reinterpret_cast<char**>(obj + 8) + 0x38);
    UnkVec3f value = x->GetAngle();
    const base::reflection::CType* type = fn_71000246e4();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value, 0);
    return r;
}
