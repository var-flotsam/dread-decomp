// CScorpiusAIComponent's unit (tools/tu_map.py): two handlers that set an id on the owner's (+0x30)
// timeline (+0x2e8) to the all-flags mask (as src/game/CKraidAIComponent/fn_7100600c98.cpp).
// fn_7100696b48 sets the flag at +0x14cc0, clears the word at +0x14cd4 and sets g_unk_710164a560
// without a null check; fn_7100695f64 first runs fn_710069409c with g_unk_71727041f8, sets
// g_unk_710164a568 when there is a timeline, then calls its own virtual at 0x518. Names are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkScorpiusKey_b59a4;
extern UnkScorpiusKey_b59a4 g_unk_710164a560;  // reached through the GOT
extern UnkScorpiusKey_b59a4 g_unk_710164a568;  // reached through the GOT
extern UnkScorpiusKey_b59a4 g_unk_71727041f8 __attribute__((visibility("hidden")));

struct UnkScorpiusAI_b59a4;
struct UnkScorpiusAIVtbl_b59a4 {
    void* slots[0x518 / 8];
    void (*vf_0x518)(UnkScorpiusAI_b59a4* self);
};

struct UnkScorpiusAI_b59a4 {
    UnkScorpiusAIVtbl_b59a4* vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;
    u8 pad_0x38[0x14cc0 - 0x38];
    bool field_0x14cc0;
    u8 pad_0x14cc1[0x14cd4 - 0x14cc1];
    u32 field_0x14cd4;
};

void fn_710069409c(UnkScorpiusAI_b59a4* self, const UnkScorpiusKey_b59a4* id, bool flag, f32 a, f32 b);

void fn_7100696b48(UnkScorpiusAI_b59a4* self) {
    self->field_0x14cc0 = true;
    self->field_0x14cd4 = 0;
    auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a560), &mask.id, false, -1.0f);
}

void fn_7100695f64(UnkScorpiusAI_b59a4* self) {
    fn_710069409c(self, &g_unk_71727041f8, false, -1.0f, -1.0f);
    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a568), &mask.id, false, -1.0f);
    }
    self->vtable->vf_0x518(self);
}
