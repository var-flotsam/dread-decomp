// CAbilityEnergyWave unit: returns the component named `id`, or null. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CAbilityLayout.h"
#include "types.h"

using base::global::CStrId;

static CAbility** FindSlot(CAbility** it, CAbility** end, CStrId key) {
    if (it == end)
        return end;
    if ((*it)->def->name.data == key.data)
        return it;
    for (++it; it != end; ++it) {
        if ((*it)->def->name.data == key.data)
            return it;
    }
    return end;
}

CAbility* fn_71003013ec(CAbilityComponent* self, const CStrId* id) {
    CAbility** end = self->end();
    CAbility** it = FindSlot(self->begin(), end, *id);
    if (it == self->end())
        return nullptr;
    return *it;
}
