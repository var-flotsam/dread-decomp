// Unit 785 (tools/tu_map.py: 0x71004f3560-0x71004f40f8, between CEmmyForestAIComponent's and
// CCharClassEmmyLabAIComponent's units; no reflected class): sets "ice_cone_audioloop" (on) or
// "ice_cone_audiostop" (off) on the owner's (+0x8) timeline (+0x2e8) to the all-flags mask; returns false.
// Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CName.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkIceCone_b61a2 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
};

bool fn_71004f3614(UnkIceCone_b61a2* self, bool on) {
    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        if (on) {
            base::reflection::UnkReflTypeIdArg mask;
            base::global::CName id("ice_cone_audioloop");
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, &id.hash, &mask.id, false, -1.0f);
        } else {
            base::reflection::UnkReflTypeIdArg mask;
            base::global::CName id("ice_cone_audiostop");
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, &id.hash, &mask.id, false, -1.0f);
        }
    }
    return false;
}
