// CYamplotXStepAttack unit (tools/tu_map.py): hands the owner's target (+0x298 of the actor at +0x8) a
// kind-5 block (game_s2/UnkParams.h) facing along the third column of the actor's matrix, with 1. The
// same block as fn_710068cf4c (src/game/behaviortree/CScorpiusRegereratingMaskAction/
// CScorpiusRegereratingMaskAction_b38.cpp). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkActor.h"
#include "game_s2/UnkParams.h"
#include "types.h"

// The kind-5 block with its first fields seen as words.
struct UnkStepParams_yxs {
    void** vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u8 field_0x14;
    u8 pad_0x15[3];
    u32 field_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u8 field_0x20[16];
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    u8 pad_0x38;
    u8 field_0x39;
    u8 pad_0x3a;
    u8 field_0x3b;
    u8 pad_0x3c;
    u8 field_0x3d;
};

struct UnkStepAttack_yxs {
    u8 pad_0x0[8];
    UnkActor* actor;  // 0x8
};

static inline u32 AsU32_yxs(const f32& value) {
    u32 bits;
    __builtin_memcpy(&bits, &value, 4);
    return bits;
}

void fn_71006edb88(UnkStepAttack_yxs* self) {
    UnkActor* actor = self->actor;
    if (void* target = actor->field_0x298) {
        UnkStepParams_yxs params;
        params.field_0x1c = 0;
        __builtin_memset(&params.field_0x8, 0, 13);
        params.field_0x30 = 0;
        __builtin_memset(params.field_0x20, 0, 16);
        params.vtable = g_unk_7101a25b10 + 2;
        params.field_0x39 = 0;
        params.field_0x3b = 0;
        params.field_0x3d = 0;
        params.field_0x34 = 5;
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
        u32 x = AsU32_yxs(m.m[2]);
        u32 y = AsU32_yxs(m.m[6]);
        u32 z = AsU32_yxs(m.m[10]);
        params.field_0x8 = x;
        params.field_0xc = y;
        params.field_0x10 = z;
        params.field_0x14 = 1;
        fn_71006f4b0c(target, &params, 1);
    }
}
