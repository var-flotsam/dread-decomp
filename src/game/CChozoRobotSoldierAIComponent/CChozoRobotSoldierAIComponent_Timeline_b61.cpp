// CChozoRobotSoldierAIComponent's unit (tools/tu_map.py): two more timeline-set callers on the owner's (+0x30)
// timeline (+0x2e8), with the all-flags mask. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkRobotSoldierKey_b61a2;
extern UnkRobotSoldierKey_b61a2 g_unk_71016497f8;  // reached through the GOT
extern UnkRobotSoldierKey_b61a2 g_unk_7101649808;  // reached through the GOT
extern UnkRobotSoldierKey_b61a2 g_unk_7101649810;  // reached through the GOT
extern f32 g_unk_7101a38b20;                       // reached through the GOT

struct UnkRobotSoldierAI_b61a2 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x7];
    bool field_0x3f;
    u8 pad_0x40[0x111c - 0x40];
    f32 field_0x111c;
    u8 pad_0x1120[0x14b1c - 0x1120];
    f32 field_0x14b1c;
};

// Arms the timer at +0x14b1c (-1) and sets g_unk_7101649808, then, when `also`, g_unk_7101649810.
void fn_7100471554(UnkRobotSoldierAI_b61a2* self, bool also) {
    self->field_0x14b1c = -1.0f;
    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_7101649808), &mask.id, false, -1.0f);
        if (also) {
            base::reflection::UnkReflTypeIdArg mask2;
            mask2.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_7101649810), &mask2.id, false, -1.0f);
        }
    }
}

// When the timer at +0x14b1c is armed (negative), clears it and sets g_unk_71016497f8; then stores the horizontal
// distance to the actor the owner acts against (fn_7100d7c990) in g_unk_7101a38b20 (1150 when under 850), and when
// +0x3f is set raises +0x111c to at least 0.1.
void fn_710047363c(UnkRobotSoldierAI_b61a2* self) {
    if (self->field_0x14b1c < 0.0f) {
        self->field_0x14b1c = 0.0f;
        if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_71016497f8), &mask.id, false, -1.0f);
        }
    }
    f32 x = self->owner->field_0x44;
    f32 distance = __builtin_fabsf(x - fn_7100d7c990(self->owner)->field_0x44);
    if (!(distance >= 850.0f))
        distance = 1150.0f;
    g_unk_7101a38b20 = distance;
    if (self->field_0x111c < 0.1f && self->field_0x3f)
        self->field_0x111c = 0.1f;
}
