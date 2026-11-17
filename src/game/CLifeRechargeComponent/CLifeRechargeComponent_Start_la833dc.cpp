// CLifeRechargeComponent's unit (tools/tu_map.py unit 1592): after the base step, triggers the three
// effects of the owner's world and plays the recharge sound with its fixed parameters.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CFXComponent.h"
#include "game/CUsableComponent.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e);

// The parameters a sound is played with.
struct UnkSoundParams_la833dc {
    u32 field_0x0;
    u32 field_0x4;
    u64 field_0x8;
    u32 field_0x10;
    CStrId id;            // 0x18
    CFilePathStrId path;  // 0x20
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4c;
    f32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;
    f32 field_0x5c;
    f32 field_0x60;
    f32 field_0x64;
    f32 field_0x68;
    bool field_0x6c;
    bool field_0x6d;
    bool field_0x6e;

    UnkSoundParams_la833dc()
        : field_0x0(0), field_0x4(0), field_0x8(0), field_0x10(0), id(g_unk_7101d470e0), path(g_unk_7101d470e8),
          field_0x28(0.0f), field_0x2c(0.0f), field_0x30(0.0f), field_0x34(-1.0f), field_0x38(1.0f),
          field_0x3c(500.0f), field_0x40(2000.0f), field_0x44(1.0f), field_0x48(0.0f), field_0x4c(0.0f),
          field_0x50(0.0f), field_0x54(-1.0f), field_0x58(-1.0f), field_0x5c(-1.0f), field_0x60(0.5f),
          field_0x64(5.0f), field_0x68(10.0f), field_0x6c(false), field_0x6d(false), field_0x6e(false) {}
    ~UnkSoundParams_la833dc();  // fn_710034e0b8
};
static_assert(__builtin_offsetof(UnkSoundParams_la833dc, field_0x6e) == 0x6e, "layout");

struct UnkLifeRechargeEffect_la833dc {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkLifeRechargeOwner_la833dc {
    u8 pad_0x0[0x280];
    UnkWorld* world;
};

struct UnkLifeRecharge_la833dc {
    u8 pad_0x0[0x30];
    UnkLifeRechargeOwner_la833dc* owner;
    u8 pad_0x38[0x540 - 0x38];
    UnkLifeRechargeEffect_la833dc* effect_0x540;
    UnkLifeRechargeEffect_la833dc* effect_0x548;
    UnkLifeRechargeEffect_la833dc* effect_0x550;
};

// The base step, as the ammo recharge's start declares it.
struct UnkAmmoRecharge_la77790;

void fn_7100a833dc(UnkLifeRecharge_la833dc* self) {
    fn_7100aa2850(reinterpret_cast<UnkAmmoRecharge_la77790*>(self));
    UnkWorld* world = self->owner->world;
    fn_710114fe8c(world, self->effect_0x548->hash, 1, 1);
    fn_710114fe8c(world, self->effect_0x540->hash, 1, 1);
    fn_710114fe8c(world, self->effect_0x550->hash, 1, 1);
    static const CFilePathStrId s_sound("props/energyrecharge/energy_recharge.wav", true);
    static UnkSoundParams_la833dc s_params;
    s_params.field_0x0 = 0;
    s_params.path = s_sound;
    s_params.field_0x6d = true;
    s_params.field_0x38 = 1.0f;
    s_params.field_0x3c = 5000.0f;
    s_params.field_0x40 = 50000.0f;
    s_params.field_0x44 = 1.0f;
    fn_7100d7c374(reinterpret_cast<UnkEntityWithSound*>(self->owner), &s_params, &g_unk_7101d470e0, false, true,
                  true);
}
