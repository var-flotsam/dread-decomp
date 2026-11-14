// CAbilityEnergyWave's unit (the name from reflection): a reflected getter (the first parameter is unused) that
// answers, as a bool value, whether any ability of the object's list (+0x40, count +0x48) has a state (CAbility's,
// state 7 deferring to the ability at +0x20) of 2 or more. Names and layouts are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAbilityLayout.h"
#include "types.h"

using base::reflection::UnkReflValue;

// Whether any ability's state is 2 or more.
static inline bool AnyActive(const CAbilityComponent* self) {
    for (CAbility** it = self->begin(); it != self->end(); ++it)
        if ((u32)(*it)->State() >= 2)
            return true;
    return false;
}

UnkReflValue fn_7100302aa4(void*, char* obj) {
    CAbilityComponent* owner = *reinterpret_cast<CAbilityComponent**>(obj + 8);
    bool value = AnyActive(owner);
    const base::reflection::CType* type = fn_710000d228();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value, false);
    return r;
}
