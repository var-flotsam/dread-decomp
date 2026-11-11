// CInfesterBallAIComponent's unit (tools/tu_map.py): the death step, once (+0x14704). Clears
// +0x1470e, turns the owner's (+0x30) collider g_unk_71726ecb78 off, sets the state +0x14708 to 2,
// hands a kind-5 parameter block to the owner's +0x298 (fn_71006f4b0c), sets the id
// g_unk_710164a120 on the owner's timeline (+0x2e8) to the all-flags mask, sends "death" to the
// owner's +0x220 (as src/game_s2/UnkSpitSend.h) and schedules fn_7100d7b054 in 0.1 s.
// Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTU2523.h"
#include "game/UnkTimelineCalls.h"
#include "game_s2/UnkParams.h"
#include "types.h"

struct UnkInfesterBallKey_b60;
extern UnkInfesterBallKey_b60 g_unk_710164a120;  // reached through the GOT
extern base::global::CStrId g_unk_71726ecb78 __attribute__((visibility("hidden")));

struct UnkInfesterBallOwner_b60 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x298 - 0x228];
    void* field_0x298;
    UnkCollisionContainer* field_0x2a0;
    u8 pad_0x2a8[0x2e8 - 0x2a8];
    void* field_0x2e8;
};

struct UnkInfesterBallDeathAI_b60 {
    u8 pad_0x0[0x30];
    UnkInfesterBallOwner_b60* owner;
    u8 pad_0x38[0x14704 - 0x38];
    bool dead;
    u8 pad_0x14705[3];
    u32 state;
    u8 pad_0x1470c[2];
    bool field_0x1470e;
};

void fn_71005d0530(UnkInfesterBallDeathAI_b60* self) {
    if (self->dead)
        return;
    base::reflection::UnkReflTypeIdArg mask1;
    UnkParams params;
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask2;
    self->field_0x1470e = false;
    if (UnkCollisionObject* object = fn_71008640a4(self->owner->field_0x2a0, &g_unk_71726ecb78))
        fn_7100d35a98(reinterpret_cast<UnkEntry*>(object), false);
    if (self->state != 2)
        self->state = 2;
    params.field_0x1c = 0;
    __builtin_memset(params.field_0x8, 0, 13);
    __builtin_memset(params.field_0x20, 0, 16);
    params.field_0x30 = 0;
    params.field_0x34 = 5;
    params.vtable = g_unk_7101a25b10 + 2;
    params.field_0x39 = 0;
    params.field_0x3d = 0;
    *reinterpret_cast<u16*>(&params.pad_0x3a) = 0x101;
    fn_71006f4b0c(self->owner->field_0x298, &params, 0);
    self->dead = true;
    UnkInfesterBallOwner_b60* owner = self->owner;
    if (auto* timeline = static_cast<UnkTimelineComponent*>(owner->field_0x2e8)) {
        mask1.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a120), &mask1.id, false, -1.0f);
        owner = self->owner;
    }
    if (void* target = owner->field_0x220) {
        u64 hash = fn_7100001570("death", 5);

        mode.id = 0;
        mask2.id = UnkAll_SpitSend().value;
        fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask2);
        owner = self->owner;
    }
    fn_7100d7b054(owner, 0.1f);
}
