// CCharClassPlayPlayerTimelineEvent (unit from tools/tu_map.py, the name from reflection): when the
// event's timeline id (+0x10) is set and the player (fn_7100dec788) has a timeline component
// (+0x2e8), sets it to the all-flags mask. A mode at +0x18 (1 or 2) must equal the game manager's
// while fn_7100df5c38 holds for the player. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkPlayer;

struct UnkTimelineNameData_b60b {
    base::global::UnkStringData base;
    u8 unk_0xc[0x24];
    u64 hash;
};

struct UnkPlayPlayerTimelineEvent_b60 {
    u8 unk_0x0[0x10];
    UnkTimelineNameData_b60b* timeline;
    s32 mode;
};

void fn_7100bc61c4(UnkPlayPlayerTimelineEvent_b60* self) {
    if (&self->timeline->base == &g_unk_7101d47108->empty)
        return;
    UnkActor* player = reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return;
    auto* timeline = static_cast<UnkTimelineComponent*>(player->field_0x2e8);
    if (!timeline)
        return;
    if (self->mode != 0 && fn_7100df5c38(g_unk_7101d47310, player)) {
        UnkGameManager* manager = g_unk_7101d47310;
        if (manager->field_0xc8 == 0 && !manager->field_0x10c)
            return;
        switch (self->mode) {
        case 1:
            if (manager->field_0xc0 != 1)
                return;
            break;
        case 2:
            if (manager->field_0xc0 != 2)
                return;
            break;
        }
    }
    base::reflection::UnkReflTypeIdArg mask;
    u64 key = self->timeline->hash;
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, &key, &mask.id, false, -1.0f);
}
