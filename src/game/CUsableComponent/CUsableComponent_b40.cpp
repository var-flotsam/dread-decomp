// CUsableComponent unit (tools/tu_map.py, the name from reflection): a reflected function of the
// component at 8 of the object that reads a bool flag (default true) from the argument's stream and,
// when it changes, stores it at +0xfa and runs an update and one of two virtuals; it returns the int 1
// as a reflected value. Names are placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

void fn_7100a9f7f0(char* self);

UnkReflValue fn_7100aa41d0(void*, char* obj, UnkReflValue* const* args) {
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
    bool v = value;
    bool* flag = reinterpret_cast<bool*>(target + 0xfa);
    if (*flag != v) {
        *flag = v;
        fn_7100a9f7f0(target);
        void** vtable = *reinterpret_cast<void***>(target);
        if (v)
            reinterpret_cast<void (*)(char*)>(vtable[0x2c0 / 8])(target);
        else
            reinterpret_cast<void (*)(char*)>(vtable[0x2c8 / 8])(target);
    }
    s32 one = 1;
    const base::reflection::CType* type = fn_7100014768();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &one, false);
    return r;
}
