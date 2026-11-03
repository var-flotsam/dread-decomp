// CEmmySancAIComponent unit (tools/tu_map.py, the name from reflection): a reflected function, the same
// code as fn_7100adec94 (CTunableMenuHelperClass), that passes its first two arguments (null when an
// argument has no type) to fn_71004ffc38 on the object at 8 and returns the empty value. Names are
// placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

void fn_71004ffc38(char* self, void* a, void* b);

static inline __attribute__((always_inline)) void* ArgPtr_b41a1(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_71005003fc(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    void* p0 = ArgPtr_b41a1(&a[0]);
    void* p1 = ArgPtr_b41a1(&a[1]);
    fn_71004ffc38(target, p0, p1);
    return base::reflection::refl_EmptyValue();
}
