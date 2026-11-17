// CAmmoRechargeComponent's unit (tools/tu_map.py unit 1583): after the base step, triggers the three
// effects of the owner's world and plays the recharge sound with its fixed parameters.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CFXComponent.h"
#include "game/CUsableComponent.h"
#include "game/UnkTU2523.h"
#include "game/sound/UnkSoundDesc.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;


struct UnkAmmoRechargeEffect_la77790 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkAmmoRechargeOwner_la77790 {
    u8 pad_0x0[0x280];
    UnkWorld* world;
};

struct UnkAmmoRecharge_la77790 {
    u8 pad_0x0[0x30];
    UnkAmmoRechargeOwner_la77790* owner;
    u8 pad_0x38[0x540 - 0x38];
    UnkAmmoRechargeEffect_la77790* effect_0x540;
    UnkAmmoRechargeEffect_la77790* effect_0x548;
    UnkAmmoRechargeEffect_la77790* effect_0x550;
};

void fn_7100a77790(UnkAmmoRecharge_la77790* self) {
    fn_7100aa2850(self);
    UnkWorld* world = self->owner->world;
    fn_710114fe8c(world, self->effect_0x548->hash, 1, 1);
    fn_710114fe8c(world, self->effect_0x540->hash, 1, 1);
    fn_710114fe8c(world, self->effect_0x550->hash, 1, 1);
    static const CFilePathStrId s_sound("props/ammorecharge/ammo_recharge.wav", true);
    static UnkSoundDesc s_params;
    s_params.kind.field_0x0 = 0;
    s_params.path = s_sound;
    s_params.field_0x6d = true;
    s_params.field_0x38 = 1.0f;
    s_params.field_0x3c = 5000.0f;
    s_params.field_0x40 = 50000.0f;
    s_params.field_0x44 = 1.0f;
    fn_7100d7c374(reinterpret_cast<UnkEntityWithSound*>(self->owner), &s_params, &g_unk_7101d470e0, false, true,
                  true);
}
