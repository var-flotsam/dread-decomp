// CAbilityEnergyWave's unit (tools/tu_map.py unit 429): whether the ability is in state 4 (the state at
// +0xb4, or while it is 7 the current state object's id at +0x20/+0x10) and the owner's inventory (+0xc0,
// +0x30, +0x270) holds the item ITEM_CENTRAL_UNIT_ENERGY (fn_71011428bc), as
// src/game/CSamusGunComponent/CSamusGunComponent_EnergyKey_b52.cpp tests it. Names and layouts are
// placeholders.
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "types.h"

struct UnkInventory;

static inline const UnkQualifiedId& GetQualifiedId_A2_b71(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

struct UnkEnergyWaveState_A2_b71 {
    u8 pad_0x0[0x10];
    u32 id;  // 0x10
};

struct UnkEnergyWaveOwner_A2_b71 {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;  // 0x270
};

struct UnkEnergyWaveActor_A2_b71 {
    u8 pad_0x0[0x30];
    UnkEnergyWaveOwner_A2_b71* owner;  // 0x30
};

struct UnkEnergyWave_A2_b71 {
    u8 pad_0x0[0x20];
    UnkEnergyWaveState_A2_b71* state;  // 0x20
    u8 pad_0x28[0x8c];
    u32 state_id;  // 0xb4
    u8 pad_0xb8[0x8];
    UnkEnergyWaveActor_A2_b71* actor;  // 0xc0
};

bool fn_7100304bd4(UnkEnergyWave_A2_b71* self) {
    u32 id = self->state_id;
    if (id == 7) {
        if (!self->state)
            return false;
        id = self->state->id;
    }
    if (id == 4) {
        UnkInventory* inventory = self->actor->owner->inventory;
        if (fn_71011428bc(inventory, GetQualifiedId_A2_b71(g_unk_717275ac18)))
            return true;
    }
    return false;
}
