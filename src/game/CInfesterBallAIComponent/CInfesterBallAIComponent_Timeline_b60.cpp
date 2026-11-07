// CInfesterBallAIComponent's unit (tools/tu_map.py): runs the base handler (fn_710110cca4); when the
// argument's object (+0x8) has its part at +0x10, sets a flag (+0x1470e) and the id
// g_unk_710164a120 on the owner's (+0x30) timeline (+0x2e8) to the all-flags mask (as
// src/game/CRinkaAIComponent/CRinkaAIComponent_Update_b59.cpp); otherwise hands over to
// fn_71005cfa94. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

void fn_710110cca4(void* self, void* arg);

struct UnkInfesterBallKey_b60;
extern UnkInfesterBallKey_b60 g_unk_710164a120;  // reached through the GOT

struct UnkInfesterBallAI_b60 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x1470e - 0x38];
    bool field_0x1470e;
};

struct UnkInfesterBallObj_b60 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkInfesterBallArg_b60 {
    u8 pad_0x0[0x8];
    UnkInfesterBallObj_b60* obj;
};

void fn_71005cfa94(UnkInfesterBallAI_b60* self, bool flag);

void fn_71005d0884(UnkInfesterBallAI_b60* self, UnkInfesterBallArg_b60* arg) {
    fn_710110cca4(self, arg);  // the base gets `arg` too (x1 untouched in the original)
    if (arg->obj && arg->obj->field_0x10) {
        self->field_0x1470e = true;
        if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a120), &mask.id, false, -1.0f);
        }
        return;
    }
    fn_71005cfa94(self, false);
}
