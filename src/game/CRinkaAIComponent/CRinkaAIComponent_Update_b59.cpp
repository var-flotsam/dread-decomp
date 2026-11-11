// CRinkaAIComponent's unit (tools/tu_map.py): runs the base step (fn_710110cca4); when the owner's
// (+0x30) object from fn_71004dad10 is there and the flag at +0x146c0 is set, calls fn_71007c21e8 on
// it with its part at +0x40; then sets the id g_unk_710164a498 on the owner's timeline (+0x2e8) to the
// all-flags mask (as src/game/CKraidAIComponent/fn_7100600c98.cpp). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CDropComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU2702.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

char* fn_71004dad10(UnkActor* owner);

struct UnkRinkaKey_b59a4;
extern UnkRinkaKey_b59a4 g_unk_710164a498;  // reached through the GOT

struct UnkRinkaAI_b59a4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x146c0 - 0x38];
    bool field_0x146c0;
};

// `arg` (x1) reaches the base step untouched in the original (tools/forwarded_args.py).
void fn_710065e014(UnkRinkaAI_b59a4* self, void* arg) {
    fn_710110cca4(self, arg);
    char* p = fn_71004dad10(self->owner);
    if (p && self->field_0x146c0)
        fn_71007c21e8(p, p + 0x40, nullptr);
    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a498), &mask.id, false, -1.0f);
    }
}
