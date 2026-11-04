// CSluggerAcidBallMovementComponent's unit (tools/tu_map.py): a melee hit on the ball. When the hit info's
// (+0x8) holder (+0x10) has a melee component (+0x2f8) that plays a melee track (fn_710092b604) and the
// ball's value at +0x5dc is at least 0.001, the id g_unk_710164a780 is set on the owner's (+0x30) timeline
// (+0x2e8) to the all-flags mask (as CSluggerAcidBallMovementComponent_Hit_b59.cpp). Names and layouts are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkMeleeComponent;
bool fn_710092b604(UnkMeleeComponent* self);

struct UnkSluggerBallMeleeOther_b60a4 {
    u8 pad_0x0[0x2f8];
    UnkMeleeComponent* melee;
};

struct UnkSluggerBallMeleeHolder_b60a4 {
    u8 pad_0x0[0x10];
    UnkSluggerBallMeleeOther_b60a4* other;
};

struct UnkSluggerBallMeleeInfo_b60a4 {
    u8 pad_0x0[0x8];
    UnkSluggerBallMeleeHolder_b60a4* holder;
};

struct UnkSluggerBallKey_b59a4;
extern UnkSluggerBallKey_b59a4 g_unk_710164a780;  // reached through the GOT

struct UnkSluggerBallMelee_b60a4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x5dc - 0x38];
    f32 field_0x5dc;
};

void fn_71006c7d1c(UnkSluggerBallMelee_b60a4* self, UnkSluggerBallMeleeInfo_b60a4* info) {
    UnkSluggerBallMeleeHolder_b60a4* holder = info->holder;
    if (!holder)
        return;
    UnkSluggerBallMeleeOther_b60a4* other = holder->other;
    if (!other || !other->melee || !fn_710092b604(other->melee))
        return;
    if (self->field_0x5dc < 0.001f)
        return;
    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a780), &mask.id, false, -1.0f);
    }
}
