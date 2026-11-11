// CChozoWarriorAttack's unit (tools/tu_map.py unit 728): fn_7100493b00, whether the attack may start: the
// base check (fn_7100360b84) must pass, and then the player's power bomb gun (the item named at +0x4c8 of
// the reflection singleton) must not answer vf_0x60 (any missing link allows the attack). Names and
// layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAttack.h"
#include "game/CChozoWarriorAttack.h"
#include "game/CGunComponent.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkStruct_710083e9d4;
struct UnkItem_710083e9d4;

// The gun the item list answers, as this file uses it.
struct UnkChozoWarriorGun_ovb0125 : base::reflection::UnkReflObject {
    bool vf_0x60() {
        return reinterpret_cast<bool (*)(UnkChozoWarriorGun_ovb0125*)>((*reinterpret_cast<void***>(this))[0x60 / 8])(this);
    }
};

struct UnkChozoWarriorPlayer_ovb0125 {
    u8 pad_0x0[0x2a8];
    UnkStruct_710083e9d4* guns;  // 0x2a8
};

struct UnkChozoWarriorPlayerLink_ovb0125 {
    u8 pad_0x0[0x10];
    UnkChozoWarriorPlayer_ovb0125* player;
};

static inline const u64* PowerBombId() {
    return reinterpret_cast<const u64*>(reinterpret_cast<const char*>(&base::reflection::refl_GetSingleton()) + 0x4c8);
}

bool fn_7100493b00(CAttack* self, void* arg, u32 mode) {
    if (!fn_7100360b84(self, arg, mode))
        return false;
    auto* link = *reinterpret_cast<UnkChozoWarriorPlayerLink_ovb0125**>(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x29a0);
    if (!link || !link->player || !link->player->guns)
        return true;
    auto* gun = reinterpret_cast<UnkChozoWarriorGun_ovb0125*>(fn_710083e9d4(link->player->guns, PowerBombId(), false));
    if (!gun || !fn_710009d50c(gun->vf_0x0(), fn_71008556fc()))
        return true;
    return !gun->vf_0x60();
}
