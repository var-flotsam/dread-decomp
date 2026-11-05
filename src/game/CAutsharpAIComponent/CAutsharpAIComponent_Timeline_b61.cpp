// CAutsharpAIComponent's unit (tools/tu_map.py): two switches on the owner's (+0x30) timeline (+0x2e8). Turning on
// sets the first id to the all-flags mask; turning off removes it and sets the second id instead.
// Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkAutsharpKey_b61a2;
extern UnkAutsharpKey_b61a2 g_unk_7101649270;  // reached through the GOT
extern UnkAutsharpKey_b61a2 g_unk_7101649278;  // reached through the GOT
extern UnkAutsharpKey_b61a2 g_unk_7101649280;  // reached through the GOT
extern UnkAutsharpKey_b61a2 g_unk_7101649288;  // reached through the GOT

struct UnkAutsharpAI_b61a2 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
};

// Turning on sets `on_id` to the all-flags mask; turning off removes it and sets `off_id`. A macro, not an inline
// function: the inliner would give the two masks lifetime markers.
#define AUTSHARP_SWITCH_B61A2(self, on, on_id, off_id)                                                            auto* timeline = static_cast<UnkTimelineComponent*>((self)->owner->field_0x2e8);                             if (!timeline)                                                                                                   return;                                                                                                  goto start;                                                                                                  base::reflection::UnkReflTypeIdArg on_mask;                                                                  base::reflection::UnkReflTypeIdArg off_mask;                                                             start:                                                                                                           if (on) {                                                                                                        on_mask.id = UnkAll_SpitSend().value;                                                                        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&(on_id)), &on_mask.id, false, -1.0f);              } else {                                                                                                         fn_7100a2b7b8(timeline, reinterpret_cast<const u64*>(&(on_id)));                                             off_mask.id = UnkAll_SpitSend().value;                                                                       fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&(off_id)), &off_mask.id, false, -1.0f);            }

void fn_71003ca540(UnkAutsharpAI_b61a2* self, bool on) {
    AUTSHARP_SWITCH_B61A2(self, on, g_unk_7101649270, g_unk_7101649278)
}

void fn_71003cbd2c(UnkAutsharpAI_b61a2* self, bool on) {
    AUTSHARP_SWITCH_B61A2(self, on, g_unk_7101649280, g_unk_7101649288)
}
