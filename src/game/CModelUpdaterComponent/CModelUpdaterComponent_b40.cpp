// CModelUpdaterComponent unit (tools/tu_map.py, the name from reflection): reflected functions that
// read three floats from the argument's stream into a vector of the object at 8 (at +0x100 or
// +0x10c), run the virtual at 0xa8, and return the int 1 as a reflected value. Names are placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "base/global/timeline/CEvent.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU218.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct Vec3_b40a3 {
    f32 x, y, z;
};

template <u32 Offset>
static inline __attribute__((always_inline)) UnkReflValue ReadVector_b40a3(char* obj, UnkReflValue* const* args) {
    UnkReflValue* arg = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    void* stream;
    if (!arg->type)
        stream = nullptr;
    else if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        stream = arg->field_0x0;
    else
        stream = arg;
    {
        f32 x, y, z;
        fn_710007cf30(stream, "fff", &x, &y, &z);
        *reinterpret_cast<Vec3_b40a3*>(target + Offset) = Vec3_b40a3{x, y, z};
    }
    void** vtable = *reinterpret_cast<void***>(target);
    reinterpret_cast<void (*)(char*, f32)>(vtable[0xa8 / 8])(target, 0.0f);
    s32 one = 1;
    const base::reflection::CType* type = fn_7100014768();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &one, false);
    return r;
}

UnkReflValue fn_71011747bc(void*, char* obj, UnkReflValue* const* args) {
    return ReadVector_b40a3<0x100>(obj, args);
}

UnkReflValue fn_7101174880(void*, char* obj, UnkReflValue* const* args) {
    return ReadVector_b40a3<0x10c>(obj, args);
}

// Reads a name and a flag from the argument's stream and, when the object at 8 has the component at
// +0x50, passes the name's hash and the flag to fn_7100199420 on the component's owner; returns 1
// when it did, else 0.
UnkReflValue fn_71011745dc(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* arg = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    void* stream;
    if (!arg->type)
        stream = nullptr;
    else if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        stream = arg->field_0x0;
    else
        stream = arg;
    char name[0x80];
    bool flag;
    fn_710007cf30(stream, "sb", name, &flag);
    s32& done = *reinterpret_cast<s32*>(name);
    s32 v = 0;
    if (*reinterpret_cast<char**>(target + 0x50)) {
        u64 hash;
        fn_710006663c(&hash, name);
        if (char* c = *reinterpret_cast<char**>(target + 0x50))
            fn_7100199420(*reinterpret_cast<void**>(*reinterpret_cast<char**>(c + 0x138) + 0x78), &hash, flag);
        v = 1;
    }
    done = v;
    const base::reflection::CType* type = fn_7100014768();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &done, false);
    return r;
}
