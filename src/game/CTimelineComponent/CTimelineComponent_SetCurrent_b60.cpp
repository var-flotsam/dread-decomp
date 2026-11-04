// CTimelineComponent (unit from tools/tu_map.py): when the id at +0x50 is set (not -1), sets it on
// the component itself to the all-flags mask. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkTimelineCurrent_b60 {
    u8 unk_0x0[0x50];
    u64 id;
};

void fn_7100a2c020(UnkTimelineCurrent_b60* self) {
    if (self->id == static_cast<u64>(-1))
        return;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(reinterpret_cast<UnkTimelineComponent*>(self), &self->id, &mask.id, false, -1.0f);
}
