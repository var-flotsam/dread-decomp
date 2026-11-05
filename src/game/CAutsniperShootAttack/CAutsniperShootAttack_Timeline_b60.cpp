// CAutsniperShootAttack's unit (tools/tu_map.py): on the owner's (+0x8) timeline (+0x2e8), when there is
// one, removes the id g_unk_71016492d8 and sets g_unk_71016492e0 to the all-flags mask.
// Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

extern u8 g_unk_71016492d8;  // reached through the GOT (declared as in src/reflection_statics/statics_00.cpp)
struct UnkAutsniperKey_b60a2;
extern UnkAutsniperKey_b60a2 g_unk_71016492e0;  // reached through the GOT

struct UnkAutsniperTimeline_b60a2 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
};

void fn_71003d7b78(UnkAutsniperTimeline_b60a2* self) {
    auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
    if (timeline) {
        fn_7100a2b7b8(timeline, reinterpret_cast<const u64*>(&g_unk_71016492d8));
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_71016492e0), &mask.id, false, -1.0f);
    }
}

struct UnkAutsniperKey2_b60a2;
extern UnkAutsniperKey2_b60a2 g_unk_71016492b0;  // reached through the GOT
extern UnkAutsniperKey2_b60a2 g_unk_71016492b8;  // reached through the GOT

// When the timeline knows the id `has` but not `hasnt`, removes `from` and sets `to` to the all-flags mask.
static inline void Swap_b60a2(UnkAutsniperTimeline_b60a2* self, u64 has, u64 hasnt, UnkAutsniperKey2_b60a2* from,
                              UnkAutsniperKey2_b60a2* to) {
    auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
    if (!timeline)
        return;
    {
        u64 a = has;
        if (!fn_7100a2be94(timeline, &a))
            return;
        u64 b = hasnt;
        if (fn_7100a2be94(timeline, &b))
            return;
    }
    fn_7100a2b7b8(timeline, reinterpret_cast<const u64*>(from));
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(to), &mask.id, false, -1.0f);
}

void fn_71003d6f20(UnkAutsniperTimeline_b60a2* self) {
    Swap_b60a2(self, 0x7c12591761ae3931ull, 0x972931e4e86855f6ull, &g_unk_71016492b8, &g_unk_71016492b0);
}

void fn_71003d7a38(UnkAutsniperTimeline_b60a2* self) {
    Swap_b60a2(self, 0x972931e4e86855f6ull, 0x7c12591761ae3931ull, &g_unk_71016492b0, &g_unk_71016492b8);
}
