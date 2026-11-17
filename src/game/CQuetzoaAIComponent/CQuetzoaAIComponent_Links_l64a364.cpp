// CQuetzoaAIComponent's unit (tools/tu_map.py, unit 973): after the base's step (fn_71003acb5c), keeps weak
// handles (+0x14ac0, +0x14ac8) to what fn_7100382150 finds for the actors of two lazily resolved links, then
// sets g_unk_710164a3c8 on the owner's timeline (+0x2e8) to the all-flags mask. Names and layouts are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkRefHandle.h"
#include "game/CBossAIComponent.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkActor;
struct UnkMagnetPathHolder_b52;
UnkMagnetPathHolder_b52* fn_7100382150(UnkActor* actor);

struct UnkQuetzoaTimelineKey_l64a364;
extern UnkQuetzoaTimelineKey_l64a364 g_unk_710164a3c8;  // reached through the GOT

// A handle to an actor, resolved from its reflected source on first use.
struct UnkQuetzoaLink_l64a364 {
    u8 pad_0x0[0x8];
    u8 source[0x28];
    u32 state;
    u32 pad_0x34;
    base::UnkRefHandle handle;

    UnkWeakHandle* Handle() {
        if (state == 1) {
            {
                UnkRefPtr fresh = fn_710023b580(source);
                fn_7100234f78(&handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            state = 2;
        }
        return reinterpret_cast<UnkWeakHandle*>(handle.ptr);
    }

    UnkActor* Actor() {
        UnkWeakHandle* h = Handle();
        return h ? reinterpret_cast<UnkActor*>(h->target) : nullptr;
    }
};
static_assert(sizeof(UnkQuetzoaLink_l64a364) == 0x40, "layout");

struct UnkQuetzoaOwner_l64a364 {
    u8 pad_0x0[0x2e8];
    UnkTimelineComponent* timeline;  // 0x2e8
};

struct UnkQuetzoaAI_l64a364 {
    u8 pad_0x0[0x30];
    UnkQuetzoaOwner_l64a364* owner;  // 0x30
    u8 pad_0x38[0x14a30 - 0x38];
    UnkQuetzoaLink_l64a364 first_link;   // 0x14a30
    u8 pad_0x14a70[0x8];
    UnkQuetzoaLink_l64a364 second_link;  // 0x14a78
    u8 pad_0x14ab8[0x8];
    UnkWeakHandle* first;   // 0x14ac0
    UnkWeakHandle* second;  // 0x14ac8
};
static_assert(__builtin_offsetof(UnkQuetzoaAI_l64a364, second) == 0x14ac8, "layout");

void fn_710064a364(UnkQuetzoaAI_l64a364* self) {
    fn_71003acb5c(reinterpret_cast<UnkCoreXAI_A2_b73*>(self));
    if (self->first_link.Actor() && fn_7100382150(self->first_link.Actor())) {
        auto* found = reinterpret_cast<UnkWeakTarget*>(fn_7100382150(self->first_link.Actor()));
        AssignRef(&self->first, GetWeakHandle(found));
    }
    if (self->second_link.Actor() && fn_7100382150(self->second_link.Actor())) {
        auto* found = reinterpret_cast<UnkWeakTarget*>(fn_7100382150(self->second_link.Actor()));
        AssignRef(&self->second, GetWeakHandle(found));
    }
    if (UnkTimelineComponent* timeline = self->owner->timeline) {
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a3c8), &mask.id, false, -1.0f);
    }
}
