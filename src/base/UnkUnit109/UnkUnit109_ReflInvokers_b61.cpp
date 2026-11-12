// Unit 109 (tools/tu_map.py, no name hint): reflected functions' invokers. fn_710012580c passes its
// two arguments (a u32 and a bool) with the object at +8 to fn_7100124170 and returns the value
// it answers by reference, described by the type at g_unk_7108b6d8f0. Names are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;


void* fn_7100124170(void* self, u32 value, bool flag);

static inline __attribute__((always_inline)) void* ArgPtr_b61(UnkReflValue* arg) {
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_710012580c(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    void* self = *reinterpret_cast<void**>(obj + 8);
    u32 value = *static_cast<const u32*>(ArgPtr_b61(&a[0]));
    bool flag = *static_cast<const bool*>(ArgPtr_b61(&a[1]));
    void* result = fn_7100124170(self, value, flag);
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    fn_71000a18d4(&r, &g_unk_7108b6d8f0, result, true);
    return r;
}
