// CTotalRechargeComponent's unit (tools/tu_map.py, unit 1602): starts the recharge. After the base step
// (fn_7100aa2850) the owner's world (+0x280 of +0x30) gets the three effects named at +0x548, +0x540 and
// +0x550, and the owner plays the recharge sound with a set of sound parameters kept in a static.
// Names and layouts are placeholders.
#include "base/UnkAtExit.h"
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

// The parameters a sound is played with: its file and how it fades and attenuates.
struct UnkSoundParams_la972d0 {
    u32 field_0x0;
    u32 field_0x4;
    u64 field_0x8;
    u32 field_0x10;
    CStrId id;            // 0x18
    CFilePathStrId path;  // 0x20
    u64 field_0x28;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
    u64 field_0x48;
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

    UnkSoundParams_la972d0()
        : field_0x0(0), field_0x4(0), field_0x8(0), field_0x10(0), id(g_unk_7101d470e0), path(g_unk_7101d470e8),
          field_0x28(0), field_0x30(0.0f), field_0x34(-1.0f), field_0x38(1.0f), field_0x3c(500.0f),
          field_0x40(2000.0f), field_0x44(1.0f), field_0x48(0), field_0x50(0.0f), field_0x54(-1.0f),
          field_0x58(-1.0f), field_0x5c(-1.0f), field_0x60(0.5f), field_0x64(5.0f), field_0x68(10.0f),
          field_0x6c(false), field_0x6d(false), field_0x6e(false) {}
    ~UnkSoundParams_la972d0();  // fn_710034e0b8
};
static_assert(__builtin_offsetof(UnkSoundParams_la972d0, field_0x6e) == 0x6e, "layout");

struct UnkWorld;

struct UnkTotalRechargeOwner_la972d0 {
    u8 pad_0x0[0x280];
    UnkWorld* world;  // 0x280
};

struct UnkTotalRecharge_la972d0 {
    u8 pad_0x0[0x30];
    UnkTotalRechargeOwner_la972d0* owner;  // 0x30
    u8 pad_0x38[0x508];
    CStrId effect_0x540;
    CStrId effect_0x548;
    CStrId effect_0x550;
};
static_assert(__builtin_offsetof(UnkTotalRecharge_la972d0, effect_0x540) == 0x540, "layout");

// The recharge base's start, as src/game/CAmmoRechargeComponent/CAmmoRechargeComponent_Start_la77790.cpp
// declares it.
struct UnkAmmoRecharge_la77790;

void fn_7100a972d0(UnkTotalRecharge_la972d0* self) {
    fn_7100aa2850(reinterpret_cast<UnkAmmoRecharge_la77790*>(self));
    UnkWorld* world = self->owner->world;
    fn_710114fe8c(world, self->effect_0x548.data->Key(), 1, 1);
    fn_710114fe8c(world, self->effect_0x540.data->Key(), 1, 1);
    fn_710114fe8c(world, self->effect_0x550.data->Key(), 1, 1);

    static const CFilePathStrId s_path("props/totalrecharge/totalrecharge.wav", true);
    static UnkSoundParams_la972d0 s_params;
    s_params.field_0x0 = 0;
    s_params.path = s_path;
    s_params.field_0x6d = true;
    s_params.field_0x38 = 1.0f;
    s_params.field_0x3c = 5000.0f;
    s_params.field_0x40 = 50000.0f;
    s_params.field_0x44 = 1.0f;
    fn_7100d7c374(reinterpret_cast<UnkEntityWithSound*>(self->owner), &s_params, &g_unk_7101d470e0, false, true,
                  true);
}
