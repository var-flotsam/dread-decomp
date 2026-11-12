// CCharClassBaseTriggerComponent's and CTriggerComponent's units: one reflected invoker each with the same code.
// It stores in its second argument (a value: in place when it already holds a compatible type, else reset and
// rebuilt) whether a string id of the object at +8 is set (+0x9b0 and +0x1e8), and returns the int 1. Names
// are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

static inline __attribute__((always_inline)) void* ArgPtr_A6t_b63(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

template <u32 Offset>
static inline __attribute__((always_inline)) UnkReflValue HasStrId_A6t_b63(char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    char* self = *reinterpret_cast<char**>(obj + 8);
    UnkReflValue* out = static_cast<UnkReflValue*>(ArgPtr_A6t_b63(&a[1]));
    bool value = *reinterpret_cast<base::global::UnkStringData**>(self + Offset) != &g_unk_7101d47108->empty;
    const CType* t = fn_710000d228();
    const CType* cur = reinterpret_cast<const CType*>(out->type);
    if (cur && out->field_0x14 == 0 &&
        (cur == t ? *reinterpret_cast<void* const*>(reinterpret_cast<const char*>(t) + 0x48) != nullptr
                  : fn_710009d52c(t, cur))) {
        fn_71000a1cc8(out, t, &value);
    } else {
        fn_71000a1c60(out);
        fn_71000a18d4(out, t, &value, false);
    }
    s32 result = 1;
    const CType* int_type = fn_7100014768();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, int_type, &result, false);
    return r;
}

UnkReflValue fn_7100a4942c(void*, char* obj, UnkReflValue* const* args) {
    return HasStrId_A6t_b63<0x9b0>(obj, args);
}

UnkReflValue fn_7100a6452c(void*, char* obj, UnkReflValue* const* args) {
    return HasStrId_A6t_b63<0x1e8>(obj, args);
}
