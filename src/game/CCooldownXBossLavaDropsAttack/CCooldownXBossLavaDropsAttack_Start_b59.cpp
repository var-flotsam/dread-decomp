// CCooldownXBossLavaDropsAttack's unit (tools/tu_map.py): clears the counter at +0x2e8, sends the owner's
// (+0x8) character class (+0x220) a request (descriptors as UnkSpitSend), sets the owner's timeline
// (+0x2e8) value for a fixed id (g_unk_7101649b10) to the all-bits mask, then hands the block at +0x2f0 to
// the owner's CCooldownXBossAIComponent (+0x240) vf_0x838. Names are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTimelineCalls.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

extern const u64 g_unk_7101649b10;  // reached through the GOT

struct UnkLavaDropsAI_b59 : base::reflection::UnkReflObject {
    void vf_0x838(void* block) {
        reinterpret_cast<void (*)(UnkLavaDropsAI_b59*, void*)>((*reinterpret_cast<void***>(this))[0x838 / 8])(this,
                                                                                                           block);
    }
};

struct UnkLavaDropsOwner_b59 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x18];
    UnkLavaDropsAI_b59* field_0x240;
    u8 pad_0x248[0xa0];
    UnkTimelineComponent* field_0x2e8;
};

struct UnkLavaDrops_b59 {
    u8 pad_0x0[0x8];
    UnkLavaDropsOwner_b59* owner;
    u8 pad_0x10[0x2d8];
    u32 field_0x2e8;
    u8 pad_0x2ec[4];
    u8 field_0x2f0[8];
};

void fn_71004c8120(UnkLavaDrops_b59* self) {
    self->field_0x2e8 = 0;
    void* anim = self->owner->field_0x220;
    if (!anim)
        goto timeline;
    // Jumped over: no lifetime markers, so it keeps its own slot.
    base::reflection::UnkReflTypeIdArg all;
    {
        base::reflection::UnkReflTypeIdArg mode;
        mode.id = 0;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(anim, 0x667fbf1d7bb5877b, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740,
                      &mask);
    }
timeline:
    UnkTimelineComponent* timeline = self->owner->field_0x2e8;
    all.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, &g_unk_7101649b10, &all.id, false, -1.0f);
    UnkLavaDropsAI_b59* ai = self->owner->field_0x240;
    ai = fn_710009d50c(ai->vf_0x0(), fn_71004cf318()) ? ai : nullptr;
    ai->vf_0x838(self->field_0x2f0);
}
