// CCharClassPlayTimelineEventByPlayerSuit (unit from tools/tu_map.py, the name from reflection): when
// all four timeline ids (+0x10 none, +0x18 varia, +0x20 gravity, +0x28 hyper) are set, the actor
// has a timeline component (+0x2e8) and the event's mode condition holds (fn_7100bc9f90), picks the
// id of the best suit the player's inventory holds (ITEM_*_SUIT amounts above zero; no id when
// there is no player or inventory) and sets it to the value of the track's action (+0x70), or to
// the all-flags mask when there is no track or action. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CInventoryComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

template <u32 Offset>
struct UnkModeCondition;
struct UnkModeActor;
bool fn_7100bc9f90(const UnkModeCondition<0x30>* self, UnkModeActor* actor);

struct UnkPlayer;
struct UnkInventory;

struct UnkTimelineNameData_b60d {
    base::global::UnkStringData base;
    u8 unk_0xc[0x24];
    u64 hash;
};

struct UnkPlayTimelineBySuitEvent_b60 {
    u8 unk_0x0[0x10];
    UnkTimelineNameData_b60d* ids[4];
};

struct UnkPlayTimelineBySuitTarget_b60 {
    UnkActor* actor;
    UnkTrack* track;
};

struct UnkSuitPlayer_b60 {
    u8 unk_0x0[0x270];
    UnkInventory* inventory;
};

struct UnkTrackActionValue_b60d {
    u8 unk_0x0[0x70];
    u32 value;
};

// An item's qualified id, built on first use.
static inline const UnkQualifiedId& ItemId_b60(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if ((u64)qid.id.data == (u64)g_unk_7101d47108 + 0x10)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

void fn_7100bc9d0c(UnkPlayTimelineBySuitEvent_b60* self, UnkPlayTimelineBySuitTarget_b60* target) {
    if (&self->ids[0]->base == &g_unk_7101d47108->empty || &self->ids[1]->base == &g_unk_7101d47108->empty ||
        &self->ids[2]->base == &g_unk_7101d47108->empty || &self->ids[3]->base == &g_unk_7101d47108->empty)
        return;
    UnkActor* actor = target->actor;
    if (!actor)
        return;
    auto* timeline = static_cast<UnkTimelineComponent*>(actor->field_0x2e8);
    if (!timeline)
        return;
    if (!fn_7100bc9f90(reinterpret_cast<const UnkModeCondition<0x30>*>(self),
                       reinterpret_cast<UnkModeActor*>(actor)))
        return;
    auto* player = reinterpret_cast<UnkSuitPlayer_b60*>(fn_7100dec788(g_unk_7101d47310));
    u64 key = static_cast<u64>(-1);
    if (player) {
        if (UnkInventory* inventory = player->inventory) {
            f32 varia = fn_7101142928(inventory, ItemId_b60(g_unk_717275a498));
            f32 gravity = fn_7101142928(inventory, ItemId_b60(g_unk_717275a458));
            f32 hyper = fn_7101142928(inventory, ItemId_b60(g_unk_717275a418));
            UnkTimelineNameData_b60d* const* id;
            if (hyper > 0.0f)
                id = &self->ids[3];
            else if (gravity > 0.0f)
                id = &self->ids[2];
            else if (varia > 0.0f)
                id = &self->ids[1];
            else
                id = &self->ids[0];
            key = (*id)->hash;
        }
    }
    const u32* value;
    if (target->track && fn_710070d500(target->track)) {
        value = &reinterpret_cast<UnkTrackActionValue_b60d*>(fn_710070d500(target->track))->value;
    } else {
        value = &UnkAll_SpitSend().value;
        asm("" : "+r"(value));  // the original takes the static's address after its guard
    }
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = *value;
    fn_7100a2cb7c(timeline, &key, &mask.id, false, -1.0f);
}
