// CChozoCommanderAttack's unit (tools/tu_map.py unit 644): fn_710042679c, whether the attack may start: the
// base check (fn_7100360b84) must pass and the Chozo commander AI's value at +0x15d10 must not be under its
// character class's +0x15b4; then the player's power bomb gun decides, as
// src/game/CChozoWarriorAttack/CChozoWarriorAttack_Check_ovb0125.cpp. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAttack.h"
#include "game/CGunComponent.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkStruct_710083e9d4;
struct UnkItem_710083e9d4;

struct UnkCommanderGun_ovb0136 : base::reflection::UnkReflObject {
    bool vf_0x60() {
        return reinterpret_cast<bool (*)(UnkCommanderGun_ovb0136*)>((*reinterpret_cast<void***>(this))[0x60 / 8])(this);
    }
};

struct UnkCommanderPlayer_ovb0136 {
    u8 pad_0x0[0x2a8];
    UnkStruct_710083e9d4* guns;  // 0x2a8
};

struct UnkCommanderPlayerLink_ovb0136 {
    u8 pad_0x0[0x10];
    UnkCommanderPlayer_ovb0136* player;
};

struct UnkCommanderCharClass_ovb0136 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x15ac];
    f32 field_0x15b4;
};

struct UnkCommanderAI_ovb0136 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x8];
    UnkCommanderCharClass_ovb0136* charClass;  // 0x10
    u8 pad_0x18[0x15d10 - 0x18];
    f32 field_0x15d10;
};

struct UnkCommanderOwner_ovb0136 {
    u8 pad_0x0[0x240];
    UnkCommanderAI_ovb0136* ai;
};

struct UnkCommanderAttack_ovb0136 {
    u8 pad_0x0[0x8];
    UnkCommanderOwner_ovb0136* owner;
};

static inline const u64* PowerBombId() {
    return reinterpret_cast<const u64*>(reinterpret_cast<const char*>(&base::reflection::refl_GetSingleton()) + 0x4c8);
}

static inline bool PowerBombAllows() {
    auto* link = *reinterpret_cast<UnkCommanderPlayerLink_ovb0136**>(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x29a0);
    if (!link || !link->player || !link->player->guns)
        return true;
    auto* gun = reinterpret_cast<UnkCommanderGun_ovb0136*>(fn_710083e9d4(link->player->guns, PowerBombId(), false));
    if (!gun || !fn_710009d50c(gun->vf_0x0(), fn_71008556fc()))
        return true;
    return !gun->vf_0x60();
}

bool fn_710042679c(CAttack* attack, void* arg, u32 mode) {
    if (!fn_7100360b84(attack, arg, mode))
        return false;
    UnkCommanderAI_ovb0136* ai = reinterpret_cast<UnkCommanderAttack_ovb0136*>(attack)->owner->ai;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_710044e708())) {
        UnkCommanderCharClass_ovb0136* charClass = ai->charClass;
        if (charClass && fn_710009d50c(charClass->vf_0x0(), fn_710044d0c0()) &&
            ai->field_0x15d10 < charClass->field_0x15b4)
            return false;
    }
    return PowerBombAllows();
}
