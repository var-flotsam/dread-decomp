// CAbilityEnergyWave unit: returns the component named `id`, or null. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CAbilityLayout.h"
#include "types.h"

using base::global::CStrId;

CAbility* fn_71003012f8(CAbilityComponent* self, const CStrId* id) {
    CAbility** it = CAbilityComponent::FindByName(self->begin(), self->end(), *id);
    if (it == self->end())
        return nullptr;
    return *it;
}
