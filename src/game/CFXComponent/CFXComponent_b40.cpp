// CFXComponent unit (tools/tu_map.py, the name from reflection): a reflected function that reads a
// name and two flags (the second defaulting to false) from the argument's stream and passes the
// name's hash and the flags to fn_710114fe8c on the object at 8; it returns the int 0 as a reflected
// value. Names are placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkWorld;
void fn_710114fe8c(UnkWorld* world, u64 hash, u32 a, u32 b);
void fn_710006663c(u64* out, const char* str);

UnkReflValue fn_710115790c(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* arg = args[0];
    UnkWorld* target = *reinterpret_cast<UnkWorld**>(obj + 8);
    void* stream;
    if (!arg->type)
        stream = nullptr;
    else if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        stream = arg->field_0x0;
    else
        stream = arg;
    {
        char name[0x80];
        bool a;
        bool b = false;
        fn_710007cf30(stream, "sbb", name, &a, &b);
        u64 hash;
        fn_710006663c(&hash, name);
        u32 fa = a;
        u32 fb = b;
        fn_710114fe8c(target, hash, fa, fb);
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
