// CAutsniperShootAttack's unit (tools/tu_map.py): after the attack base's reset (fn_71003609b8), removes two ids
// from the owner's timeline (+0x2e8) and sets a third to the all-flags mask, clears the flag at +0x27248 of the
// owner's AI component when it is an autsniper's, then runs the two follow-up steps. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAttack.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::reflection::CClass;

extern u8 g_unk_71016492c0;  // reached through the GOT (declared as in src/reflection_statics/statics_00.cpp)
extern u8 g_unk_71016492d0;  // reached through the GOT (declared as in src/reflection_statics/statics_00.cpp)
struct UnkAutsniperKey3_b61a2;
extern UnkAutsniperKey3_b61a2 g_unk_71016492e8;  // reached through the GOT

struct UnkAutsniperTimeline_b60a2;
void fn_71003d7b78(UnkAutsniperTimeline_b60a2* self);  // as CAutsniperShootAttack_Timeline_b60.cpp defines it
struct UnkSelf;
void fn_71003d5a14(UnkSelf* self, bool force);  // as src/game/CAutsniperShootAttack/fn_71003d5a14.cpp defines it

struct UnkAutsniperAIView_b61a2 {
    virtual const CClass* vf_0x0();
};

void fn_71003d82c4(UnkAttackBase* self) {
    fn_71003609b8(self);
    auto* timeline = static_cast<UnkTimelineComponent*>(self->field_0x8->field_0x2e8);
    if (timeline) {
        fn_7100a2b7b8(timeline, reinterpret_cast<const u64*>(&g_unk_71016492c0));
        fn_7100a2b7b8(timeline, reinterpret_cast<const u64*>(&g_unk_71016492d0));
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_71016492e8), &mask.id, false, -1.0f);
    }
    auto* ai = static_cast<UnkAutsniperAIView_b61a2*>(self->field_0x8->field_0x240);
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_71003da2e0()))
        reinterpret_cast<u8*>(ai)[0x27248] = 0;
    fn_71003d7b78(reinterpret_cast<UnkAutsniperTimeline_b60a2*>(self));
    fn_71003d5a14(reinterpret_cast<UnkSelf*>(self), true);
}
