// CAbilityEnergyWave's unit (tools/tu_map.py): the wave ability (state 6 active, state 7 deferring to the
// parent's): stopping it when the local gun is not busy, taking a new primary gun, and ending it. Names and
// layouts are placeholders.
#include <new>
#include "base/UnkNamedRegistry.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAbility.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityLayout.h"
#include "game/CFXComponent.h"
#include "game/CGrabComponent.h"
#include "game/CGunComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU1428.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::UnkReflValue;

struct UnkSelf;
struct UnkOwner;
struct UnkObject;
struct UnkComponent;
struct UnkPlayer;
struct UnkWorld;
struct UnkLifeComponent;
struct UnkSamusGunComponent;
struct UnkScriptCaller;
struct UnkSmLockon_A1_b64;
struct UnkSmAbility3_A2_b63;
struct UnkAbilityC_A2_b69;
struct UnkStateObject;
struct UnkEnergyWaveCompA_b49;
struct UnkEnergyWaveCompB_b49;
struct UnkEnergyWaveObject_b49;
struct UnkEnergyWaveFxComp_b49;

extern char g_unk_7101648d28[];
extern const u64 g_unk_7101648d18;

void fn_7100301600(void* self, u64 hash, bool on, UnkObject* world);

void fn_71003053c4(CAbilityEnergyWave* self);
void fn_710085ab38(UnkSamusGunComponent* gun);

struct UnkEnergyWavePlayer2_ovb22 {
    u8 pad_0x0[0x1e0];
    UnkScriptCaller* script;  // 0x1e0
    u8 pad_0x1e8[0xc0];
    UnkSamusGunComponent* gun;  // 0x2a8
};

struct UnkEnergyWaveWorldHolder_ovb22 {
    u8 pad_0x0[0x280];
    UnkObject* world;  // 0x280
};

struct UnkEnergyWaveWorldLink_ovb22 {
    u8 pad_0x0[0x10];
    UnkEnergyWaveWorldHolder_ovb22* holder;  // 0x10
};


void fn_7100305b00(CAbilityEnergyWave* self, UnkEnergyWaveGun* gun);

// Whether the ability is in state 6 (state 7 defers to the parent's).
static inline bool IsActive(CAbilityEnergyWave* self) {
    s32 state = self->state;
    if (state == 7) {
        if (!self->state_holder)
            return false;
        state = self->state_holder->state;
    }
    return state == 6;
}

// fn_71003053c4 is passed an empty tag (an undefined register) and a false flag that it ignores.
struct UnkEnergyWaveStopTag_ovb22 {};
using StopFn = void (*)(CAbilityEnergyWave*, UnkEnergyWaveStopTag_ovb22, bool);

static inline void Stop(CAbilityEnergyWave* self) {
    reinterpret_cast<StopFn>(&fn_71003053c4)(self, UnkEnergyWaveStopTag_ovb22(), false);
}

// Whether the local player (fn_7100defcd4) is `owner` and its gun component is busy (fn_710083cd4c).
static inline bool IsLocalGunBusy(UnkOwner* owner) {
    if (fn_7100defcd4(g_unk_7101d47310) != owner)
        return false;
    UnkEnergyWavePlayer2_ovb22* player = reinterpret_cast<UnkEnergyWavePlayer2_ovb22*>(owner);
    return player->gun && fn_710083cd4c(player->gun);
}

void fn_7100305760(CAbilityEnergyWave* self, UnkEnergyWavePlayer2_ovb22* player) {
    if (!IsActive(self))
        return;
    if (fn_7100df5c20(g_unk_7101d47310, player) && IsActive(self) &&
        reinterpret_cast<UnkEnergyWavePlayer2_ovb22*>(fn_7100defcd4(g_unk_7101d47310)) == player && player->gun &&
        fn_710083cd4c(player->gun))
        return;
    Stop(self);
}

// Takes a new primary gun.
void fn_7100305a24(CAbilityEnergyWave* self, UnkEnergyWaveGun* gun) {
    if (!gun)
        return;
    UnkOwner* player = self->component->Owner<UnkOwner>();
    if (!fn_710009d50c(gun->GetClass(), fn_71008570a4()))  // CPrimaryGun
        return;
    if (self->gun == gun)
        return;
    self->gun = gun;
    if (!(IsActive(self) && IsLocalGunBusy(player)))
        Stop(self);
    fn_7100305b00(self, gun);
}

// Ends the wave: the script event (unless `reason` is 2), the local gun reset to the id at +0x3a8, the
// stop, and both effects (+0x390, +0x398) turned on in the world behind the manager's +0x29a0.
void fn_71003052a0(CAbilityEnergyWave* self, s32 reason) {
    fn_71002fa068(reinterpret_cast<UnkSmLockon_A1_b64*>(self));
    if (reason != 2) {
        UnkEnergyWavePlayer2_ovb22* player = self->component->Owner<UnkEnergyWavePlayer2_ovb22>();
        fn_7100305088(player->script, "OnAbilityEWEnd", g_unk_710162e2ed);
    }
    if (UnkOwner* local = fn_7100defcd4(g_unk_7101d47310)) {
        if (UnkSamusGunComponent* gun = reinterpret_cast<UnkEnergyWavePlayer2_ovb22*>(local)->gun) {
            auto* typed = reinterpret_cast<UnkEnergyWaveGun*>(gun);
            gun = fn_710009d50c(typed->GetClass(), fn_7100857564()) ? gun : nullptr;  // CSamusGunComponent
            fn_710085ab38(gun);
            fn_710083f32c(gun, &self->gun_id, false, true, false);
            fn_710083ad6c(gun);
        }
        Stop(self);
    }
    if (UnkEnergyWaveWorldLink_ovb22* link =
            *reinterpret_cast<UnkEnergyWaveWorldLink_ovb22**>(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x29a0)) {
        if (UnkEnergyWaveWorldHolder_ovb22* holder = link->holder) {
            UnkObject* world = holder->world;
            fn_7100301600(self->component, self->fx_a->hash, true, world);
            fn_7100301600(self->component, self->fx_b->hash, true, world);
        }
    }
}
