// CGobblerAIComponent's unit (tools/tu_map.py): runs the base handler (fn_710110cca4); when the
// owner's (+0x30) slot +0x398 holds an object (+0x10) of class CGobblerSpawnPointComponent, calls
// fn_7101126af4 on it and looks up its link (+0x3f8, fn_71003b0c4c). The linked actor (+0x10) gets
// the id g_unk_7101649fa8 set to the all-flags mask on its timeline (+0x2e8) when it has one, and
// fn_7100232c68 otherwise. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBossAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU2704.h"
#include "game/UnkTU341.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkZiplineLinkTarget_b49;
struct UnkZiplineLink_b49;

struct UnkGobblerKey_b60;
extern UnkGobblerKey_b60 g_unk_7101649fa8;  // reached through the GOT

struct UnkGobblerLinkTarget_b60 {
    u8 pad_0x0[0x10];
    UnkActor* actor;
};

struct UnkGobblerSpawnPoint_b60 {
    virtual const base::reflection::CClass* vf_0x0();
    u8 pad_0x8[0x3f8 - 0x8];
    u8 link[0x40];
};

struct UnkGobblerSlot_b60 {
    u8 pad_0x0[0x10];
    UnkGobblerSpawnPoint_b60* object;
};

struct UnkGobblerOwner_b60 {
    u8 pad_0x0[0x398];
    UnkGobblerSlot_b60* field_0x398;
};

struct UnkGobblerAI_b60 {
    u8 pad_0x0[0x30];
    UnkGobblerOwner_b60* owner;
};

// `arg` (x1) reaches the base step untouched in the original (tools/forwarded_args.py).
void fn_710058252c(UnkGobblerAI_b60* self, void* arg) {
    fn_710110cca4(self, arg);
    UnkGobblerSlot_b60* slot = self->owner->field_0x398;
    if (!slot)
        return;
    UnkGobblerSpawnPoint_b60* spawn = slot->object;
    if (!spawn || !fn_710009d50c(spawn->vf_0x0(), fn_7100a19b44()))
        return;
    // The Gobbler spawn point is a spawn point component (its view here has only the members this source reads).
    fn_7101126af4(reinterpret_cast<CSpawnPointComponent*>(spawn));
    auto* target = reinterpret_cast<UnkGobblerLinkTarget_b60*>(
        *fn_71003b0c4c(reinterpret_cast<UnkZiplineLink_b49*>(spawn->link)));
    if (!target)
        return;
    UnkActor* actor = target->actor;
    if (!actor)
        return;
    if (auto* timeline = static_cast<UnkTimelineComponent*>(actor->field_0x2e8)) {
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_7101649fa8), &mask.id, false, -1.0f);
    } else {
        fn_7100232c68(actor);
    }
}
