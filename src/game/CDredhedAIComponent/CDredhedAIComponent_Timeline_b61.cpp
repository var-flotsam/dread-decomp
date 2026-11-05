// CDredhedAIComponent's unit (tools/tu_map.py): when the +0x14730 object is in state 1 (+0x600) and the flag at
// +0x146c8 is still clear (it is raised), on the owner's (+0x30) timeline (+0x2e8) removes g_unk_7101649c88 when it
// is known and sets g_unk_7101649c90 (on) or g_unk_7101649c98 (off) to the all-flags mask. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

// As CDredhedAIComponent_Timeline_b60.cpp declares them.
extern const u64 g_unk_7101649c98 __attribute__((visibility("hidden")));
extern const u64 g_unk_7101649c88 __attribute__((visibility("hidden")));
extern const u64 g_unk_7101649c90 __attribute__((visibility("hidden")));

struct UnkDredhedState_b61a2 {
    u8 pad_0x0[0x600];
    s32 field_0x600;
};

struct UnkDredhedAI_b61a2 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x146c8 - 0x38];
    bool field_0x146c8;
    u8 pad_0x146c9[0x14730 - 0x146c9];
    UnkDredhedState_b61a2* field_0x14730;
};

void fn_71004e7660(UnkDredhedAI_b61a2* self, bool on) {
    if (self->field_0x14730->field_0x600 != 1 || self->field_0x146c8)
        return;
    self->field_0x146c8 = true;
    auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
    if (!timeline)
        return;
    if (fn_7100a2be94(timeline, &g_unk_7101649c88))
        fn_7100a2b7b8(timeline, &g_unk_7101649c88);
    // Jumped over: the on mask gets no lifetime markers, so it keeps its own slot.
    goto start;
    base::reflection::UnkReflTypeIdArg on_mask;
start:
    if (on) {
        on_mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &g_unk_7101649c90, &on_mask.id, false, -1.0f);
        return;
    }
    base::reflection::UnkReflTypeIdArg off_mask;
    off_mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, &g_unk_7101649c98, &off_mask.id, false, -1.0f);
}
