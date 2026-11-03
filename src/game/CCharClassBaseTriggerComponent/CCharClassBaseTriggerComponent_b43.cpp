// CCharClassBaseTriggerComponent unit (tools/tu_map.py, the name from reflection): a reflected function
// that passes its bool first argument and its second (null when it has no type) to fn_7100030128 on the
// member at +0xa10 of the object at 8 and returns the empty value; a copy of fn_7100f0f428
// (src/game/CPlayerInfo/CPlayerInfo_b40.cpp). Names are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

// Declared as src/game/CPlayerInfo/CPlayerInfo_b40.cpp does.
void fn_7100030128(char* self, bool flag, void* arg);

static inline __attribute__((always_inline)) void* ArgPtr_b43a2(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_7100a4953c(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    bool flag = *base::reflection::refl_ArgValue<bool>(&a[0]);
    void* p1 = ArgPtr_b43a2(&a[1]);
    fn_7100030128(target + 0xa10, flag, p1);
    return base::reflection::refl_EmptyValue();
}
