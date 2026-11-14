// Finds the component named `id` in the array at +0x40 (compared with the name in the definition at
// +0xc8 of each) and returns it when it is an instance of the class fn_710033a31c describes, else null.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityLayout.h"
#include "types.h"

using base::global::CStrId;

CAbility* fn_71003005e0(CAbilityComponent* self, const CStrId* id) {
    CAbility** it = CAbilityComponent::FindByName(self->begin(), self->end(), *id);
    if (it == self->end())
        return nullptr;
    CAbility* ability = *it;
    if (!ability)
        return nullptr;
    return fn_710009d50c(ability->GetClass(), fn_710033a31c()) ? ability : nullptr;
}
