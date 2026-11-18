// CChozoWarriorXAIComponent unit: after the base start step, switches music tracks 0 and 1 over (0.2 s), sets
// 2.3 on a key of the player's ability list (+0x5a8) and, in the scenario with hash 0xee3d114b1ff93c77 while the
// player lacks the power bomb, sets id 0x2000 on the owner's and the game manager's controllers. Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/CBossAIComponent.h"
#include "game/CChozoWarriorXAIComponent.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkShakernautId.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU39.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

struct UnkChozoXOwner_l48e3e4 {
    u8 pad_0x0[0x220];
    UnkShakernautCtrl_b53* ctrl;  // 0x220
};

struct UnkChozoWarriorXAI_l48e3e4 {
    u8 pad_0x0[0x30];
    UnkChozoXOwner_l48e3e4* owner;  // 0x30
};

struct UnkChozoXAbility_l48e3e4 {
    u8 pad_0x0[0x5a8];
    u8 keys[1];  // 0x5a8
};

void fn_7100498efc(UnkChozoWarriorXAI_l48e3e4* self);
struct UnkCoreXAI_A2_b73;

static inline void* MusicManager_l48e3e4() {
    if (!g_unk_71726bc560) {
        void* m = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(m);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(m);
    }
    return g_unk_71726bc560;
}

// An item's qualified id, built on first use.
static inline const UnkQualifiedId& ItemId_l48e3e4(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

void fn_710048e3e4(UnkChozoWarriorXAI_l48e3e4* self) {
    fn_7100498efc(self);
    fn_71003aff64(reinterpret_cast<UnkCoreXAI_A2_b73*>(self), 1, 0);
    fn_7100fdcf2c(MusicManager_l48e3e4(), 0, true, 0.2f);
    fn_7100fdcf2c(MusicManager_l48e3e4(), 1, false, 0.2f);
    UnkAbility* ability = fn_7100df5928(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310));
    fn_7100030fc0(reinterpret_cast<UnkChozoXAbility_l48e3e4*>(ability)->keys, g_unk_71726cfa68, 2.3f);
    if (fn_7100de9bf8(g_unk_7101d47310)->data->Key() != 0xee3d114b1ff93c77)
        return;
    UnkInventory* inventory = static_cast<UnkInventory*>(fn_7100df55e4(g_unk_7101d47310));
    if (!inventory || fn_71011428bc(inventory, ItemId_l48e3e4(g_unk_717275a818)))  // ITEM_WEAPON_POWER_BOMB
        return;
    UnkShakernautId_b53 id;
    if (UnkShakernautCtrl_b53* ctrl = self->owner->ctrl) {
        id.value = 0x2000;
        fn_7101178564(ctrl, &id, true, false);
    }
    if (UnkChozoXOwner_l48e3e4* owner = reinterpret_cast<UnkChozoXOwner_l48e3e4*>(fn_7100defcd4(g_unk_7101d47310))) {
        if (UnkShakernautCtrl_b53* ctrl = owner->ctrl) {
            UnkShakernautId_b53 manager_id;
            manager_id.value = 0x2000;
            fn_7101178564(ctrl, &manager_id, true, false);
        }
    }
}
