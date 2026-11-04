// CChozoRobotSoldierAIComponent's unit (tools/tu_map.py): raises the flag at +0x14b79 and, when the
// owner (+0x30) has a timeline (+0x2e8), sets the id g_unk_71016497f0 to the all-flags mask.
// Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkRobotSoldierKey_b60a2;
extern UnkRobotSoldierKey_b60a2 g_unk_71016497f0;  // reached through the GOT

struct UnkRobotSoldierTimeline_b60a2 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x14b79 - 0x38];
    bool field_0x14b79;
};

void fn_7100472fd4(UnkRobotSoldierTimeline_b60a2* self) {
    self->field_0x14b79 = true;
    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_71016497f0), &mask.id, false, -1.0f);
    }
}

struct UnkRobotSoldierKey2_b60a2;
extern UnkRobotSoldierKey2_b60a2 g_unk_7101649818;  // reached through the GOT

struct UnkRobotSoldierOnce_b60a2 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x14b20 - 0x38];
    bool field_0x14b20;
};

// Once: raises the flag at +0x14b20 and sets the id g_unk_7101649818 on the owner's timeline as above.
void fn_71004738e4(UnkRobotSoldierOnce_b60a2* self) {
    if (self->field_0x14b20)
        return;
    self->field_0x14b20 = true;
    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_7101649818), &mask.id, false, -1.0f);
    }
}
