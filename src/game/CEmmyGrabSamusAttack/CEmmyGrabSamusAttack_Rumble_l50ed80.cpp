// CEmmyGrabSamusAttack's unit: plays the "grab hand two" vibration on the attack's owner.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/math/UnkZeroVector3.h"
#include "game/UnkHandle.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkHandleTarget;
struct UnkEntityWithSound;
struct UnkRumbleParams;
UnkHandle fn_7100d7c650(UnkEntityWithSound* self, const UnkRumbleParams* params, bool a, bool b, bool c);

struct UnkRumbleWords_l50ed80 {
    u32 x, y, z;
};

// What a rumble is played from (UnkRumbleParams), with its defaults.
struct UnkRumbleParams_l50ed80 {
    CFilePathStrId path;
    u64 field_0x8;
    s32 field_0x10;
    UnkRumbleWords_l50ed80 field_0x14;
    u64 field_0x20;
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    u16 field_0x40;

    UnkRumbleParams_l50ed80()
        : path(g_unk_7101d470e8), field_0x8(0), field_0x10(0x7fffffff),
          field_0x14(reinterpret_cast<const UnkRumbleWords_l50ed80&>(g_unk_710164779c)), field_0x20(0),
          field_0x28(1.0f), field_0x2c(1.0f), field_0x30(0.5f), field_0x34(0.0f), field_0x38(0.0f),
          field_0x3c(1.0f), field_0x40(0) {}
};

struct UnkEmmyGrabAttack_l50ed80 {
    u8 pad_0x0[0x8];
    UnkEntityWithSound* owner;
};

void fn_710050ed80(UnkEmmyGrabAttack_l50ed80* self) {
    static CFilePathStrId s_path("actors/emmy/grabhandtwo_emmy_01.bnvib", true);
    UnkRumbleParams_l50ed80 params;
    params.field_0x10 = 3;
    params.path = s_path;
    params.field_0x28 = 2.0f;
    fn_7100d7c650(self->owner, reinterpret_cast<const UnkRumbleParams*>(&params), false, true, true);
}
