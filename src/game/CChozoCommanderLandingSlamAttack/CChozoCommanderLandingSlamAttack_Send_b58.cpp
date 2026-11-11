// CChozoCommanderLandingSlamAttack's unit (tools/tu_map.py): clears the flag at +0x14, hands a kind-5
// parameter block (game_s2/UnkParams.h's vtable, with a rate of 1 at +0x10) to the owner's (+0x8) +0x298
// sub-object, sends the +0x220 sub-object a zero (refl_EmitU32 written out, for its stack slots) and sets the stage at +0x58 to 1.
// Names are placeholders.
#include "base/reflection/UnkReflStatics.h"
#include "game/CChozoCommanderLandingSlamAttack.h"
#include "game_s2/UnkParams.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

// The kind-5 block with the rate at +0x10 and the flag at +0x14 written separately.
struct UnkParams5Rate_b58 {
    void** vtable;
    u64 field_0x8;
    f32 field_0x10;
    u8 field_0x14;
    u8 pad_0x15[3];
    u32 field_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u8 field_0x20[16];
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    u8 field_0x38;
    u8 field_0x39;
    u8 pad_0x3a;
    u8 field_0x3b;
    u8 pad_0x3c;
    u8 field_0x3d;
};

struct UnkLandingSlamOwner_b58 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x70];
    void* field_0x298;
};

struct UnkLandingSlam_b58 {
    u8 pad_0x0[0x8];
    UnkLandingSlamOwner_b58* owner;
    u8 pad_0x10[0x4];
    u8 field_0x14;
    u8 pad_0x15[0x43];
    s32 stage;
};

void fn_7100437c44(UnkLandingSlam_b58* self) {
    self->field_0x14 = 0;
    base::reflection::UnkReflTypeIdArg value;
    UnkParams5Rate_b58 params;
    base::reflection::UnkReflTypeIdArg arg;
    params.field_0x30 = 0;
    params.vtable = g_unk_7101a25b10 + 2;
    __builtin_memset(params.field_0x20, 0, 16);
    params.field_0x3b = 0;
    params.field_0x8 = 0;
    params.field_0x3d = 0;
    params.field_0x18 = 0;
    params.field_0x1c = 1;
    params.field_0x14 = 1;
    params.field_0x34 = 5;
    params.field_0x38 = 1;
    params.field_0x39 = 1;
    params.field_0x10 = 1.0f;
    fn_71006f4b0c(self->owner->field_0x298, &params, 1);
    void* target = self->owner->field_0x220;
    value.id = 0;
    arg.id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
    fn_7101176690(target, 0xc0e10244d20037b9, 0xf4061ff25a2163d6, &value, &g_unk_710164b958, &g_unk_7172769740,
                  &arg);
    self->stage = 1;
}
