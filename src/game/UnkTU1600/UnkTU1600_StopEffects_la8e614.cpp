// CTeleporterUsableComponent's unit (tools/tu_map.py unit 1600): stops the effect of every mode in the
// mode-to-effect table (the effects fn_7100a8e4c0 starts). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkFxId_b0233.h"
#include "game/UnkTeleporterFxEntry_b0233.h"
#include "game/UnkTeleporterFx_b0233.h"
#include "game/UnkTeleporterSelf_b0233.h"
#include "types.h"

using base::global::UnkStringData;

extern UnkTeleporterFxEntry_b0233 g_unk_7172751a60[8] __attribute__((visibility("hidden")));

void DisableAllModeTableIdsOnActorComponent(UnkTeleporterSelf_b0233* self) {
#pragma clang loop unroll(full)
    for (UnkTeleporterFxEntry_b0233& entry : g_unk_7172751a60) {
        UnkFxId_b0233 name = entry.id;
        void* component = self->actor->field_0x228;
        u64 hash = name.data->Key();
        fn_71011738f8(static_cast<UnkEnergyWaveFxComp_b49*>(component), &hash, 0);
    }
}
