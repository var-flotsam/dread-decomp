// CPrimaryGun's unit (tools/tu_map.py), fn_7100857344: false while the current ability (fn_7100df5460) is in state
// 6 (the state at +0xb4, or while it is 7 the current state object's id at +0x20/+0x10) and the owner's (+0x18)
// inventory (+0x270) holds ITEM_CENTRAL_UNIT_DECAYED_ENERGY (fn_71011428bc, as
// src/game/CAbilityEnergyWave/CAbilityEnergyWave_HasCentralUnitEnergy_A2_b71.cpp tests it); otherwise the result
// of fn_7100835a14. Names and layouts are placeholders.
#include "game/CGun.h"
#include "game/CPrimaryGun.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkInventory;
bool fn_71011428bc(UnkInventory* inventory, const UnkQualifiedId& item);
extern UnkPropertyEx<> g_unk_717275ac58;  // ITEM_CENTRAL_UNIT_DECAYED_ENERGY

static inline const UnkQualifiedId& GetQualifiedId_A4_b76(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

struct UnkPrimaryGunState_A4_b76 {
    u8 pad_0x0[0x10];
    u32 id;  // 0x10
};

struct UnkPrimaryGunAbility_A4_b76 {
    u8 pad_0x0[0x20];
    UnkPrimaryGunState_A4_b76* state;  // 0x20
    u8 pad_0x28[0x8c];
    u32 state_id;  // 0xb4
};

struct UnkPrimaryGunOwner_A4_b76 {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;  // 0x270
};

struct UnkPrimaryGun_A4_b76 {
    u8 pad_0x0[0x18];
    UnkPrimaryGunOwner_A4_b76* owner;  // 0x18
};

// As src/flag_id/fn_7100843474.cpp declares it.
bool fn_7100857344(void* object) {
    UnkPrimaryGun_A4_b76* self = static_cast<UnkPrimaryGun_A4_b76*>(object);
    if (UnkPrimaryGunAbility_A4_b76* ability =
            static_cast<UnkPrimaryGunAbility_A4_b76*>(fn_7100df5460(g_unk_7101d47310))) {
        u32 id = ability->state_id;
        if (id == 7) {
            if (!ability->state)
                return fn_7100835a14(self);
            id = ability->state->id;
        }
        if (id == 6) {
            if (UnkInventory* inventory = self->owner->inventory) {
                if (fn_71011428bc(inventory, GetQualifiedId_A4_b76(g_unk_717275ac58)))
                    return false;
            }
        }
    }
    return fn_7100835a14(self);
}
