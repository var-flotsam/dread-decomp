// A reflected getter (and fn_710117364c, the same query returning a bool): looks the named entry
// up in the object's component at +0x50 and returns the translation of the 4x4 transform it fills
// in, or zero when there is no component or no entry.
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

bool fn_71001c1694(char* self, const void* name, f32* transform);

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

// The same lookup on the component itself: the translation of the named entry's transform into
// `out` (copied bitwise), and whether there was one.
template <class Owner, class Key, class Matrix, bool (*Lookup)(Owner*, const Key*, Matrix*)>
static inline bool Translation(char* self, const Key* name, u32* out) {
    f32 m[16] = {};
    m[0] = m[5] = m[10] = m[15] = 1;
    char* c = *reinterpret_cast<char**>(self + 0x50);
    if (c && Lookup(*reinterpret_cast<Owner**>(c + 0x138), name, reinterpret_cast<Matrix*>(m))) {
        u32 x = reinterpret_cast<u32*>(m)[3];
        u32 y = reinterpret_cast<u32*>(m)[7];
        u32 z = reinterpret_cast<u32*>(m)[11];
        out[0] = x;
        out[1] = y;
        out[2] = z;
        return true;
    }
    return false;
}

bool fn_710117364c(char* self, const void* name, u32* out) {
    return Translation<char, void, f32, fn_71001c1694>(self, name, out);
}

// The same by id (the bone owner's lookup fn_71001c1920), next door.
struct UnkBoneOwner;
struct UnkMatrix44;
bool fn_71001c1920(UnkBoneOwner* self, const u64* id, UnkMatrix44* out);

bool fn_71011736e0(char* self, const u64* id, u32* out) {
    return Translation<UnkBoneOwner, u64, UnkMatrix44, fn_71001c1920>(self, id, out);
}
