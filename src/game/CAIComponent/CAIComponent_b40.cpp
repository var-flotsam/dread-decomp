// CAIComponent unit (tools/tu_map.py, the name from reflection): a reflected function that passes its first two
// arguments (null when an argument has no type; inline, or behind a pointer for values over 8 bytes or
// kind 1) to fn_710110a82c on the object at 8 and returns the int 0 as a reflected value. Names are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

void fn_710110a82c(char* self, void* a, void* b);

static inline __attribute__((always_inline)) void* ArgPtr_b40a3(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_7101115b5c(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    void* p0 = ArgPtr_b40a3(&a[0]);
    void* p1 = ArgPtr_b40a3(&a[1]);
    fn_710110a82c(target, p0, p1);
    s32 v = 0;
    const base::reflection::CType* type = fn_7100014768();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &v, false);
    return r;
}
