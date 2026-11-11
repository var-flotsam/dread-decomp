// CScorpiusPoisonousSpitMovementComponent's unit (tools/tu_map.py): the update. After the base update
// (fn_7100ac3a78), while the flag at +0x5d8 is set the timer at +0x5dc grows by `dt`; once it passes
// the Scorpius tunable's (g_unk_7172702318) +0x30 plus +0x34 minus +0x38, the first time (flag at
// +0x5d9) the owner (+0x30) has a timeline (+0x2e8), the id g_unk_710164a610 is set to the all-flags
// mask (as src/game/CKraidAIComponent/fn_7100600c98.cpp). Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CProjectileMovement.h"
#include "game/UnkActor.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkNailongThornMovement;

extern UnkTunable g_unk_7172702318;

struct UnkSpitTunable_b59a4 {
    u8 pad_0x0[0x30];
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
};

struct UnkSpitKey_b59a4;
extern UnkSpitKey_b59a4 g_unk_710164a610;  // reached through the GOT

struct UnkSpitMovement_b59a4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x5d8 - 0x38];
    bool field_0x5d8;
    bool field_0x5d9;
    u8 pad_0x5da[2];
    f32 field_0x5dc;
};

void fn_7100699150(UnkSpitMovement_b59a4* self, f32 dt) {
    fn_7100ac3a78(reinterpret_cast<UnkNailongThornMovement*>(self), dt);
    if (!self->field_0x5d8)
        return;
    f32 t = self->field_0x5dc + dt;
    self->field_0x5dc = t;
    if (self->field_0x5d9)
        return;
    const UnkSpitTunable_b59a4* tunable = reinterpret_cast<const UnkSpitTunable_b59a4*>(&g_unk_7172702318);
    if (t > tunable->field_0x30 + tunable->field_0x34 - tunable->field_0x38) {
        if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
            self->field_0x5d9 = true;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a610), &mask.id, false, -1.0f);
        }
    }
}
