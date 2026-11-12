// CMissileBaseGun's unit (tools/tu_map.py): fn_710084f22c takes one of the gun's ammunition item (+0x1880) from
// the owner's inventory, telling the HUD "OnMissileEmpty" when none is left, and passes the gun's value
// (vf_0x1e0) on to +0x18a0; fn_710084eb44 is whether the gun can fire: not while the current ability is in
// state 6 below full charge (+0x3d8), only with ammunition, when fn_7100835a14 holds and the sub-area GUI cache
// counts the gun's name (+0x10). Names and layouts are placeholders.
#include "game/CBombGun.h"
#include "game/CGun.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "base/UnkVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkSubAreaGuiCache.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU39.h"
#include "types.h"

struct UnkInventory;

struct UnkMissileGunOwner_ovb0103 {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;
};

struct UnkMissileGun_ovb0103 {
    virtual void vf_0x0();
    f32 vf_0x1e0() {
        return reinterpret_cast<f32 (*)(UnkMissileGun_ovb0103*)>((*reinterpret_cast<void***>(this))[0x1e0 / 8])(this);
    }
    u64 field_0x8;
    base::global::CStrId name;  // 0x10
    UnkMissileGunOwner_ovb0103* owner;  // 0x18
    u8 pad_0x20[0x1880 - 0x20];
    UnkQualifiedId ammo;  // 0x1880
    u8 pad_0x1890[0x18a0 - 0x1890];
    u8 field_0x18a0[8];
};

// Tells the HUD (vf_0x1e8) an event by name.
static inline void SendHudEvent_ovb0103(UnkIngameMenuHud* hud, const char* name) {
    reinterpret_cast<void (*)(UnkIngameMenuHud*, const char*, bool)>((*reinterpret_cast<void***>(hud))[0x1e8 / 8])(
        hud, name, false);
}

void fn_710084f22c(UnkMissileGun_ovb0103* self) {
    if (self->ammo.id.data == &g_unk_7101d47108->empty)
        return;
    f32 amount = fn_7101142928(self->owner->inventory, self->ammo) - 1.0f;
    if (amount <= 0.0f) {
        if (UnkIngameMenuHud* hud = fn_7100deac84(g_unk_7101d47310)->field_0x2d0)
            SendHudEvent_ovb0103(hud, "OnMissileEmpty");
    }
    fn_7101142a28(self->owner->inventory, self->ammo, true, true, true, amount, 0.0f);
    f32 value = self->vf_0x1e0();
    fn_710003152c(self->field_0x18a0, &value);
}

s32 fn_710119db04(UnkSubAreaGuiCache* self, const base::global::CStrId* name);
struct UnkPrimaryGun_A4_b76;
struct UnkLockOnMissileGun_b49a4;

struct UnkMissileGunState_ovb0103 {
    u8 pad_0x0[0x10];
    s32 state;
};

struct UnkMissileGunAbility_ovb0103 {
    u8 pad_0x0[0x20];
    UnkMissileGunState_ovb0103* sub;  // 0x20
    u8 pad_0x28[0xb4 - 0x28];
    s32 kind;  // 0xb4
    u8 pad_0xb8[0x3d8 - 0xb8];
    f32 field_0x3d8;
};

static inline UnkSubAreaGuiCache* GuiCache_ovb0103() {
    UnkSubAreaGuiCache* cache = g_unk_71727254f8;
    if (!cache) {
        cache = static_cast<UnkSubAreaGuiCache*>(fn_71000839a0(0x48, 0x3f));
        fn_710119d484(cache);
        g_unk_71727254f8 = cache;
    }
    return cache;
}

bool fn_710084eb44(UnkLockOnMissileGun_b49a4* gun) {
    auto* self = reinterpret_cast<UnkMissileGun_ovb0103*>(gun);
    if (auto* ability = static_cast<UnkMissileGunAbility_ovb0103*>(fn_7100df5460(g_unk_7101d47310))) {
        s32 state = ability->kind;
        if (state == 7)
            state = ability->sub ? ability->sub->state : 0;
        if (state == 6 && !(ability->field_0x3d8 >= 1.0f))
            return false;
    }
    if (!self->owner)
        return false;
    if (self->owner->inventory && self->vf_0x1e0() <= 0.0f)
        return false;
    if (!fn_7100835a14(reinterpret_cast<UnkPrimaryGun_A4_b76*>(self)))
        return false;
    if (self->name.data == &g_unk_7101d47108->empty)
        return false;
    UnkSubAreaGuiCache* cache = GuiCache_ovb0103();
    return fn_710119db04(cache, reinterpret_cast<const base::global::CStrId*>(reinterpret_cast<u8*>(self) + 0x10)) > 0;
}
