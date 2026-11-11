// CPlayerInfo unit (tools/tu_map.py, the name from reflection): a reflected function that passes its
// bool first argument and its second (null when it has no type) to fn_7100030128 on the member at
// +0x348 of the object at 8 and returns the empty value. Names are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::reflection::UnkReflValue;

static inline __attribute__((always_inline)) void* ArgPtr_b40a3(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_7100f0f428(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    bool flag = *base::reflection::refl_ArgValue<bool>(&a[0]);
    void* p1 = ArgPtr_b40a3(&a[1]);
    fn_7100030128(target + 0x348, flag, p1);
    return base::reflection::refl_EmptyValue();
}
