// CLifeComponent unit (tools/tu_map.py, the name from reflection): a reflected function that reads
// a string from the argument's stream into the string at +0xb0 of the component at 8 and returns
// the int 0 as a reflected value. Names are placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

UnkReflValue fn_7101161298(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* arg = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    void* stream;
    if (!arg->type)
        stream = nullptr;
    else if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        stream = arg->field_0x0;
    else
        stream = arg;
    const char* str;
    fn_710007cee4(stream, &str);
    fn_710007e77c(reinterpret_cast<base::global::CRntString*>(target + 0xb0), str);
    s32 zero = 0;
    const base::reflection::CType* type = fn_7100014768();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &zero, false);
    return r;
}
