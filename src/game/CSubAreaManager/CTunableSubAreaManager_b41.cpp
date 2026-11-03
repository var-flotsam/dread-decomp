// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py, the name from reflection): a reflected
// function of the object at 8 that passes a u32 argument and a second (nullable) argument to
// fn_7100eef488 and returns the pointer it gives (typed by fn_7100ef0e00). Same shape as
// fn_71011dc794. Names are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

const base::reflection::CType* fn_7100ef0e00();
void* fn_7100eef488(char* self, u32 index, void* arg);

static inline __attribute__((always_inline)) void* ArgPtr_b41a3(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_7100ef1140(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    u32 index = *base::reflection::refl_ArgValue<u32>(&a[0]);
    void* p1 = ArgPtr_b41a3(&a[1]);
    void* v = fn_7100eef488(target, index, p1);
    const CType* type = fn_7100ef0e00();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &v, false);
    return r;
}
