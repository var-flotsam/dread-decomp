// CDredhedAIComponent's unit (tools/tu_map.py): when the +0x14730 object is in state 1 (+0x600) and the
// flag at +0x146c8 is set (it is cleared), on the owner's (+0x30) timeline (+0x2e8) removes the id
// g_unk_7101649c98 when it is known and sets g_unk_7101649c88 to the all-flags mask.
// Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

extern const u64 g_unk_7101649c98 __attribute__((visibility("hidden")));
extern const u64 g_unk_7101649c88 __attribute__((visibility("hidden")));

struct UnkDredhedState_b60a2 {
    u8 pad_0x0[0x600];
    s32 field_0x600;
};

struct UnkDredhedAI_b60a2 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x146c8 - 0x38];
    bool field_0x146c8;
    u8 pad_0x146c9[0x14730 - 0x146c9];
    UnkDredhedState_b60a2* field_0x14730;
};

void fn_71004e7b2c(UnkDredhedAI_b60a2* self) {
    if (self->field_0x14730->field_0x600 != 1 || !self->field_0x146c8)
        return;
    self->field_0x146c8 = false;
    auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
    if (!timeline)
        return;
    if (fn_7100a2be94(timeline, &g_unk_7101649c98))
        fn_7100a2b7b8(timeline, &g_unk_7101649c98);
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, &g_unk_7101649c88, &mask.id, false, -1.0f);
}
