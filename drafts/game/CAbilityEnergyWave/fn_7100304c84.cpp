// CAbilityEnergyWave unit: starts the wave. Releases the multi-lockon ability unless it is busy, stops the
// wave unless the local gun is already busy, runs the script event (unless `reason` is 2), takes the local
// gun's id (or the multi-lockon's while it is active) and selects the central unit beams by the energy the
// inventory holds; then resets the timer at +0x3c8. Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/UnkNoArg.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbility.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityLayout.h"
#include "game/CGunComponent.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkInventory;
struct UnkPlayer;
struct UnkSamusGunComponent;
struct UnkScriptCaller;

void fn_71003053c4(CAbilityEnergyWave* self, UnkNoArg, bool);
void fn_710085ab38(UnkSamusGunComponent* gun);
const base::global::CStrId& fn_7100840844(UnkSamusGunComponent*);
UnkAbility* fn_7100df59cc(UnkPlayer* self);

// The player's multi-lockon ability, as this unit reads it.
struct UnkMultiLockon_l304c84 : CAbility {
    u8 pad_0x390[0x260];
    bool field_0x5f0;
    u8 pad_0x5f1[0x7];
    base::global::CStrId field_0x5f8;
};
static_assert(__builtin_offsetof(UnkMultiLockon_l304c84, field_0x5f8) == 0x5f8, "layout");

bool fn_710031c798(UnkMultiLockon_l304c84* self);

struct UnkGunItem_l304c84 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual bool vf_0x30();
};

struct UnkEnergyWavePlayer_l304c84 {
    u8 pad_0x0[0x1e0];
    UnkScriptCaller* script;  // 0x1e0
    u8 pad_0x1e8[0x88];
    UnkInventory* inventory;  // 0x270
    u8 pad_0x278[0x30];
    UnkSamusGunComponent* gun;  // 0x2a8
};

struct UnkWaveTunable_l304c84 {
    u8 pad_0x0[0x28];
    u32 field_0x28;
};
extern UnkWaveTunable_l304c84 g_unk_7172728150;

extern base::global::CStrId g_unk_71726af0d0 __attribute__((visibility("hidden")));

static inline UnkMultiLockon_l304c84* MultiLockon() {
    return reinterpret_cast<UnkMultiLockon_l304c84*>(fn_7100df59cc(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310)));
}

static inline UnkEnergyWavePlayer_l304c84* LocalPlayer() {
    return reinterpret_cast<UnkEnergyWavePlayer_l304c84*>(fn_7100defcd4(g_unk_7101d47310));
}

// Whether the local player is `owner` and its gun is busy.
static inline bool IsLocalGunBusy(UnkOwner* owner) {
    if (fn_7100defcd4(g_unk_7101d47310) != owner)
        return false;
    auto* player = reinterpret_cast<UnkEnergyWavePlayer_l304c84*>(owner);
    return player->gun && fn_710083cd4c(player->gun);
}

// The qualified id of an item property, built on first use.
static inline const UnkQualifiedId& ItemId(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

void fn_7100304c84(CAbilityEnergyWave* self, s32 reason) {
    fn_71002f9f18(reinterpret_cast<UnkSmLockon_A1_b64*>(self), reason);
    UnkOwner* owner = self->component->Owner<UnkOwner>();

    UnkMultiLockon_l304c84* lockon = MultiLockon();
    if (lockon && lockon->State() == 6 && !fn_710031c798(lockon) && !lockon->field_0x5f0)
        lockon->Virtual(0xa0, true);

    if (!(self->State() == 6 && IsLocalGunBusy(owner)))
        fn_71003053c4(self, UnkNoArg(), false);

    if (reason != 2)
        fn_7100305088(reinterpret_cast<UnkEnergyWavePlayer_l304c84*>(owner)->script, "OnAbilityEWBegin", g_unk_710162e2ed);
    UnkEnergyWavePlayer_l304c84* local = LocalPlayer();
    if (UnkSamusGunComponent* gun = local->gun) {
        auto* typed = reinterpret_cast<UnkEnergyWaveGun*>(gun);
        gun = fn_710009d50c(typed->GetClass(), fn_7100857564()) ? gun : nullptr;  // CSamusGunComponent
        fn_710085ab38(gun);
        self->gun_id = fn_7100840844(gun);
        lockon = MultiLockon();
        if (lockon && lockon->State() == 6)
            self->gun_id = lockon->field_0x5f8;

        if (fn_7101142928(local->inventory, ItemId(g_unk_717275ac18)) > 0.0f &&
            fn_7101142928(local->inventory, ItemId(g_unk_717275ac58)) <= 0.0f) {
            fn_710083f32c(gun, &GameManagerIdTable().field_0x1850, true, true, false);
            fn_710083f32c(gun, &g_unk_71726af0d0, false, true, false);
        } else if (fn_7101142928(local->inventory, ItemId(g_unk_717275ac58)) > 0.0f &&
                   reinterpret_cast<UnkGunItem_l304c84*>(
                       fn_710083e9d4(reinterpret_cast<UnkStruct_710083e9d4*>(gun),
                                     reinterpret_cast<const u64*>(&g_unk_71726af0d0), false))
                       ->vf_0x30()) {
            fn_710083f32c(gun, &g_unk_71726af0d0, false, true, false);
        }
    }
    self->field_0x3c8 = g_unk_7172728150.field_0x28;
}
