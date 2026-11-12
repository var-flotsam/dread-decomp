// CAbilityEnergyWave's unit (the name from reflection): every ability of the owner's list (+0x40, count +0x48)
// whose definition (+0xc8) has the non-empty string id `id` at +0x20 is switched on (fn_71002f9694 with 1; the
// list is re-read after each), then the id is passed on to the map data (fn_7100e8d8b0, fn_7100e9c43c).
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CAbility.h"
#include "game/UnkTU2566.h"
#include "types.h"

using base::global::CStrId;

struct UnkMapData;

struct UnkAbilityDef_A2_b69 {
    u8 pad_0x0[0x20];
    CStrId id;  // +0x20
};

struct UnkAbilityC_A2_b69 {
    u8 pad_0x0[0xc8];
    UnkAbilityDef_A2_b69* def;  // +0xc8
};

struct UnkAbilityOwnerC_A2_b69 {
    u8 pad_0x0[0x40];
    UnkAbilityC_A2_b69** abilities;  // +0x40
    u32 count;                       // +0x48
};

void fn_7100301860(UnkAbilityOwnerC_A2_b69* self, const CStrId* id) {
    for (UnkAbilityC_A2_b69** it = self->abilities; it != self->abilities + self->count; ++it) {
        UnkAbilityC_A2_b69* ability = *it;
        UnkAbilityDef_A2_b69* def = ability->def;
        base::global::UnkStringData* empty = &g_unk_7101d47108->empty;
        base::global::UnkStringData* data = def->id.data;
        base::global::UnkStringData* wanted = id->data;
        if (!((data == empty) | (data != wanted)))
            fn_71002f9694(ability, true);
    }
    fn_7100e9c43c(fn_7100e8d8b0(), id);
}
