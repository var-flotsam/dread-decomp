// CTunableMenuHelperClass unit (tools/tu_map.py, the name from reflection): a reflected function
// that passes its first two arguments (null when an argument has no type) to fn_7100addddc on the
// object at 8 and returns the empty value. Names are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

void fn_7100addddc(char* self, void* a, void* b);

static inline __attribute__((always_inline)) void* ArgPtr_b40a3(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_7100adec94(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    void* p0 = ArgPtr_b40a3(&a[0]);
    void* p1 = ArgPtr_b40a3(&a[1]);
    fn_7100addddc(target, p0, p1);
    return base::reflection::refl_EmptyValue();
}
