// A reflected getter: looks the named entry up in the object's component at +0x50 and returns
// the translation of the 4x4 transform it fills in, or zero when there is no component or no
// entry.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

bool fn_71001c1694(char* self, const void* name, f32* transform);
const CType* fn_71000246e4();

UnkReflValue fn_7101174944(void*, char* obj, UnkReflValue* const* args) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    UnkReflValue* arg = args[0];
    const void* name;
    if (!arg->type)
        name = nullptr;
    else if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        name = arg->field_0x0;
    else
        name = arg;
    f32 m[16] = {};
    m[0] = m[5] = m[10] = m[15] = 1;
    char* c = *reinterpret_cast<char**>(target + 0x50);
    // The CVector3D is copied bitwise, through integer registers.
    u32 x, y, z;
    if (c && fn_71001c1694(*reinterpret_cast<char**>(c + 0x138), name, m)) {
        x = reinterpret_cast<u32*>(m)[3];
        y = reinterpret_cast<u32*>(m)[7];
        z = reinterpret_cast<u32*>(m)[11];
    } else {
        x = y = z = 0;
    }
    alignas(8) u32 v[3] = {x, y, z};
    const CType* type = fn_71000246e4();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &v, false);
    return r;
}
