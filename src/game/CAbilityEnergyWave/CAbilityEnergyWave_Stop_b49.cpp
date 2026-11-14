// CAbilityEnergyWave (tools/tu_map.py): stops the wave. Unless the game manager's flag at +0x1af0 is
// set, releases the object at +0x68 (its slot 0x90), detaches the owner's (+0x30) two components
// (+0x260 with 0, +0x210 with that object) and refreshes the items (fn_71002fda50 with true, false);
// then calls slot 0x78 on every child (+0x40, count +0x48), clears +0xcc, sets +0xd8 to -1 and, when
// the world behind the manager's +0x29a0 (+0x10, +0x280) has the object with hash 0xb1489dc93c1d38dd,
// releases it (fn_710114fe8c, as in fn_7100300bd0.cpp). Names and layouts are placeholders.
#include "game/CAbilityEnergyWave.h"
#include "game/CFXComponent.h"
#include "game/CGrabComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1428.h"
#include "game/CAbilityLayout.h"
#include "types.h"

struct UnkWorld;
struct UnkPart;
struct UnkEnergyWaveCompA_b49;
struct UnkEnergyWaveCompB_b49;
struct UnkEnergyWaveObject_b49;

struct UnkEnergyWaveOwner_b49 {
    u8 pad_0x0[0x210];
    UnkEnergyWaveCompB_b49* field_0x210;
    u8 pad_0x218[0x48];
    UnkEnergyWaveCompA_b49* field_0x260;
};

struct UnkWorldHolder_b49 {
    u8 pad_0x0[0x280];
    UnkWorld* world;
};
struct UnkWorldLink_b49 {
    u8 pad_0x0[0x10];
    UnkWorldHolder_b49* holder;
};

void fn_71002fdfac(CAbilityComponent* self) {
    if (!reinterpret_cast<u8*>(g_unk_7101d47310)[0x1af0]) {
        if (self->current) {
            self->current->Virtual(0x90);
            self->current = nullptr;
        }
        if (self->Owner<UnkEnergyWaveOwner_b49>()->field_0x260)
            fn_710113f270(self->Owner<UnkEnergyWaveOwner_b49>()->field_0x260, nullptr);
        if (self->Owner<UnkEnergyWaveOwner_b49>()->field_0x210)
            fn_710094f8f0(self->Owner<UnkEnergyWaveOwner_b49>()->field_0x210,
                          reinterpret_cast<UnkEnergyWaveObject_b49*>(self->current));
        fn_71002fda50(self, true, false);
    }
    for (CAbility **it = self->begin(), **end = self->end(); it != end; ++it)
        (*it)->Virtual(0x78);
    self->field_0xcc = 0.0f;
    self->energy = -1.0f;
    if (UnkWorldLink_b49* link = *reinterpret_cast<UnkWorldLink_b49**>(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x29a0)) {
        if (UnkWorldHolder_b49* holder = link->holder) {
            if (UnkWorld* world = holder->world) {
                if (fn_7101152b90(world, 0xb1489dc93c1d38ddull))
                    fn_710114fe8c(world, 0xb1489dc93c1d38ddull, 0, 1);
            }
        }
    }
}
