// CCooldownXBossLavaDropsAttack's unit (tools/tu_map.py): for every pending drop of the owner's
// CCooldownXBossAIComponent whose handle is gone, clears its flag, counts it, sets the timeline value of its
// kind, sends the matching track to the owner's character class and flags its name on +0x228. Names and
// layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::UnkStringData;
using base::reflection::UnkReflObject;

extern const u64 g_unk_7101649ae0;  // reached through the GOT
extern const u64 g_unk_7101649ae8;  // reached through the GOT
extern const u64 g_unk_7101649af0;  // reached through the GOT
extern const u64 g_unk_7101649af8;  // reached through the GOT

struct UnkLavaDrop_l4c7324 {
    UnkStringData* kind;  // 0x0
    u8 pad_0x8[0x8];
    UnkStringData* name;  // 0x10
    u8 pad_0x18[0x18];
    UnkWeakHandle* handle;  // 0x30
    bool pending;  // 0x38
    u8 pad_0x39[0x7];
};
static_assert(sizeof(UnkLavaDrop_l4c7324) == 0x40, "layout");

struct UnkLavaDropList_l4c7324 {
    UnkLavaDrop_l4c7324* items;
    u8 pad_0x8[0x8];
    u32 count;  // 0x10
    u8 pad_0x14[0x14];
    u32 released;  // 0x28
};

struct UnkLavaDropsAI_l4c7324 {
    u8 pad_0x0[0x15658];
    UnkLavaDropList_l4c7324 drops;
};

struct UnkLavaDropsAttack_l4c7324 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x310 - 0x10];
    u32 field_0x310;
    u32 field_0x314;
    u32 field_0x318;
    u32 field_0x31c;
};

// Sets the timeline value `id` to the all-bits mask, through the caller's `all`.
static inline void SetTimeline(UnkTimelineComponent* timeline, const u64* id,
                               base::reflection::UnkReflTypeIdArg& all) {
    if (timeline) {
        all.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, id, &all.id, false, -1.0f);
    }
}

void fn_71004c7324(UnkLavaDropsAttack_l4c7324* self) {
    UnkReflObject* object = static_cast<UnkReflObject*>(self->owner->field_0x240);
    object = fn_710009d50c(object->vf_0x0(), fn_71004cf318()) ? object : nullptr;
    UnkLavaDropList_l4c7324& drops = reinterpret_cast<UnkLavaDropsAI_l4c7324*>(object)->drops;
    UnkLavaDrop_l4c7324* end = drops.items + drops.count;
    for (UnkLavaDrop_l4c7324* drop = drops.items; drop != end; ++drop) {
        if (drop->handle && drop->handle->target)
            continue;
        if (!drop->pending)
            continue;
        drop->pending = false;
        ++drops.released;
        UnkActor* owner = self->owner;
        base::reflection::UnkReflTypeIdArg mode;
        base::reflection::UnkReflTypeIdArg mask;
        if (void* anim = owner->field_0x220) {
            base::reflection::UnkReflTypeIdArg all;
            UnkTimelineComponent* timeline = static_cast<UnkTimelineComponent*>(owner->field_0x2e8);
            u64 hash;
            switch (drop->kind->Key()) {
            case 0xfc0afa18dd48957d:
                hash = self->field_0x314 == 0 ? 0x1cfdbc6faa7f4a29 : 0xe8d53f87e3f3e673;
                SetTimeline(timeline, &g_unk_7101649ae0, all);
                break;
            case 0x913c5999cc2002f8:
                hash = self->field_0x310 == 0 ? 0x0dc31c0a76197830 : 0xf0d80dd1919e8775;
                SetTimeline(timeline, &g_unk_7101649ae8, all);
                break;
            case 0x8b59361142c681d2:
                hash = self->field_0x318 == 0 ? 0xa9f0ff6d720bfe40 : 0x0545408cbe07ff42;
                SetTimeline(timeline, &g_unk_7101649af8, all);
                break;
            case 0x8bb4f4733ec20d8c:
                hash = self->field_0x31c == 0 ? 0xbb18d90e0480187b : 0xa1e4ca2101676902;
                SetTimeline(timeline, &g_unk_7101649af0, all);
                break;
            default:
                hash = ~0ull;
                break;
            }
            u64 kind = drop->kind->Key();
            mode.id = 0;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(anim, hash, kind, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
            owner = self->owner;
        }
        if (void* target = owner->field_0x228) {
            u64 name = drop->name->Key();
            fn_7101172780(static_cast<UnkFlagTarget*>(target), reinterpret_cast<const char*>(&name));
        }
    }
}
