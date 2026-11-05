// CKraidTripleFlyingSpikesAttack's unit (tools/tu_map.py): after the attack's base step
// (fn_71005f2018, a thunk to fn_71003608ac), when the owner's (+0x8) AI component (+0x240) is a
// CKraidAIComponent it gets fn_71005ff8f0; then the word at +0x30 is cleared and the ids
// g_unk_710164a1e0 and g_unk_710164a1e8 are set on the owner's timeline (+0x2e8) to the all-flags
// mask (as src/game/CKraidAIComponent/fn_7100600c98.cpp). Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkKraidSpikesKey_b59a4;
extern const UnkKraidSpikesKey_b59a4 g_unk_710164a1e0 __attribute__((visibility("hidden")));
extern const UnkKraidSpikesKey_b59a4 g_unk_710164a1e8 __attribute__((visibility("hidden")));

struct UnkKraidSpikesOwner_b59a4 {
    u8 pad_0x0[0x240];
    base::reflection::UnkReflObject* ai;
    u8 pad_0x248[0x2e8 - 0x248];
    UnkTimelineComponent* field_0x2e8;
};

struct UnkKraidSpikesAttack_b59a4 {
    u8 pad_0x0[0x8];
    UnkKraidSpikesOwner_b59a4* owner;
    u8 pad_0x10[0x20];
    u32 field_0x30;
};

void fn_71005f2018(char* self);  // the base restart (a thunk to fn_71003608ac)
void fn_71005ff8f0(base::reflection::UnkReflObject* ai);

void fn_71005f7d60(UnkKraidSpikesAttack_b59a4* self) {
    base::reflection::UnkReflTypeIdArg second;
    base::reflection::UnkReflTypeIdArg first;
    fn_71005f2018(reinterpret_cast<char*>(self));
    base::reflection::UnkReflObject* ai = self->owner->ai;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_71005fac74()))
        fn_71005ff8f0(ai);
    self->field_0x30 = 0;
    if (UnkTimelineComponent* timeline = self->owner->field_0x2e8) {
        first.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a1e0), &first.id, false, -1.0f);
    }
    if (UnkTimelineComponent* timeline = self->owner->field_0x2e8) {
        second.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a1e8), &second.id, false, -1.0f);
    }
}
