// SLogicPathNode unit (tools/tu_map.py, the name from reflection): a reflected function that passes its u32 first
// argument and its second (null when it has no type) to fn_71010590dc on the object at 8 and returns the
// reflected object it gives, by reference, described by its dynamic class. Names are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;

UnkReflObject* fn_71010590dc(char* self, u32 index, void* arg);

static inline __attribute__((always_inline)) void* ArgPtr_b40a3(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_710105d0d0(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    u32 index = *base::reflection::refl_ArgValue<u32>(&a[0]);
    void* p1 = ArgPtr_b40a3(&a[1]);
    UnkReflObject* o = fn_71010590dc(target, index, p1);
    const CType* type = reinterpret_cast<const CType*>(o->vf_0x0());
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    fn_71000a18d4(&r, type, o, true);
    return r;
}
