// CQuetzoaEnergyWaveMovementComponent's unit (tools/tu_map.py): clears the word at +0x5e4, sets the
// state at +0x5ec to 2, runs fn_71007d24cc on the owner's (+0x30) object from fn_71003be154 when there
// is one, then sets the id g_unk_710164a3d8 on the owner's timeline (+0x2e8) to the all-flags mask
// (as src/game/CKraidAIComponent/fn_7100600c98.cpp). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

char* fn_71003be154(UnkActor* owner);
void fn_71007d24cc(char* self);

struct UnkEnergyWaveKey_b59a4;
extern UnkEnergyWaveKey_b59a4 g_unk_710164a3d8;  // reached through the GOT

struct UnkEnergyWave_b59a4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x5e4 - 0x38];
    u32 field_0x5e4;
    u8 pad_0x5e8[4];
    u32 field_0x5ec;
};

void fn_710064ca40(UnkEnergyWave_b59a4* self) {
    self->field_0x5e4 = 0;
    self->field_0x5ec = 2;
    if (char* p = fn_71003be154(self->owner))
        fn_71007d24cc(p);
    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a3d8), &mask.id, false, -1.0f);
    }
}
