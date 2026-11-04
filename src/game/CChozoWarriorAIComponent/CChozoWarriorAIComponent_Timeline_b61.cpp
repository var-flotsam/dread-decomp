// CChozoWarriorAIComponent's unit (tools/tu_map.py): records `on` at +0x1499a and sets g_unk_7101649a08 (on) or
// g_unk_7101649a10 (off) on the owner's (+0x30) timeline (+0x2e8) to the all-flags mask. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkWarriorKey_b61a2;
extern UnkWarriorKey_b61a2 g_unk_7101649a08;  // reached through the GOT
extern UnkWarriorKey_b61a2 g_unk_7101649a10;  // reached through the GOT

struct UnkWarriorAI_b61a2 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x1499a - 0x38];
    bool field_0x1499a;
};

void fn_710049ae88(UnkWarriorAI_b61a2* self, bool on) {
    self->field_0x1499a = on;
    auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
    if (!timeline)
        return;
    // Jumped over: the on mask gets no lifetime markers, so it keeps its own slot.
    goto start;
    base::reflection::UnkReflTypeIdArg on_mask;
start:
    if (on) {
        on_mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_7101649a08), &on_mask.id, false, -1.0f);
        return;
    }
    base::reflection::UnkReflTypeIdArg off_mask;
    off_mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_7101649a10), &off_mask.id, false, -1.0f);
}
