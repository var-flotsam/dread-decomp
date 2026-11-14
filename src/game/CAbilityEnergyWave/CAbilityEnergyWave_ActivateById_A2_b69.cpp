// CAbilityEnergyWave's unit (the name from reflection): every ability of the owner's list (+0x40, count +0x48)
// whose definition (+0xc8) has the non-empty string id `id` at +0x20 is switched on (fn_71002f9694 with 1; the
// list is re-read after each), then the id is passed on to the map data (fn_7100e8d8b0, fn_7100e9c43c).
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CAbility.h"
#include "game/CAbilityLayout.h"
#include "game/UnkTU2566.h"
#include "types.h"

using base::global::CStrId;

struct UnkMapData;

void fn_7100301860(CAbilityComponent* self, const CStrId* id) {
    for (CAbility** it = self->begin(); it != self->end(); ++it) {
        CAbility* ability = *it;
        UnkAbilityDef* def = ability->def;
        base::global::UnkStringData* empty = &g_unk_7101d47108->empty;
        base::global::UnkStringData* data = def->id.data;
        base::global::UnkStringData* wanted = id->data;
        if (!((data == empty) | (data != wanted)))
            fn_71002f9694(reinterpret_cast<UnkAbilityC_A2_b69*>(ability), true);
    }
    fn_7100e9c43c(fn_7100e8d8b0(), id);
}
