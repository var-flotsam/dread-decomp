// CPlayerMovement unit (tools/tu_map.py, the name from reflection): a reflected function that reads
// a bool (default true) from the argument's stream, passes it to the virtual at 0x348 of the object at
// 8, and returns the int 0 as a reflected value. Names are placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkVec2_b40a3 {
    f32 x, y;
};

void fn_710006c598(void* self, const base::global::CStrId* id, const UnkVec2_b40a3* v);

UnkReflValue fn_71010dcf24(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* arg = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    void* stream;
    if (!arg->type)
        stream = nullptr;
    else if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        stream = arg->field_0x0;
    else
        stream = arg;
    bool value = true;
    fn_710007cf30(stream, "b", &value);
    void** vtable = *reinterpret_cast<void***>(target);
    reinterpret_cast<void (*)(char*, bool)>(vtable[0x348 / 8])(target, value);
    s32 zero = 0;
    const base::reflection::CType* type = fn_7100014768();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &zero, false);
    return r;
}

// Reads a name and two floats (default zero) from the argument's stream and passes the name's id
// and the pair to fn_710006c598 on the member at +0x708 of the object at 8; returns the int 0.
UnkReflValue fn_71010dcfd0(void*, char* obj, UnkReflValue* const* args) {
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
        char name[0x80];
        UnkVec2_b40a3 v = {};
        fn_710007cf30(stream, "sff", name, &v.x, &v.y);
        base::global::CStrId id(name, true);
        fn_710006c598(target + 0x708, &id, &v);
    }
    s32 zero = 0;
    const base::reflection::CType* type = fn_7100014768();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &zero, false);
    return r;
}
