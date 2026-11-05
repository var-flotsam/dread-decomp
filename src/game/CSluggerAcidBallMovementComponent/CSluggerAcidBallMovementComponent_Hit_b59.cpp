// CSluggerAcidBallMovementComponent's unit (tools/tu_map.py): the hit test. A null `other` or the one
// held at +0x230 (+0x10) is no hit; one that fn_7100d7b50c accepts and whose melee component (+0x2f8)
// plays a melee track (fn_710092b604) sets the id g_unk_710164a780 on the owner's (+0x30) timeline
// (+0x2e8) to the all-flags mask (as src/game/CKraidAIComponent/fn_7100600c98.cpp) and hits; anything
// else goes to the base test fn_7100ac4ec0 (as in
// src/game/CKraidAcidBlobsMovementComponent/CKraidAcidBlobsMovementComponent.cpp). Names and layouts
// are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkObj;
bool fn_7100ac4ec0(UnkObj* self, void* a1, void* a2, void* a3, void* a4, void* a5);
struct UnkMeleeComponent;
bool fn_710092b604(UnkMeleeComponent* self);

struct UnkSluggerBallOther_b59a4 {
    u8 pad_0x0[0x2f8];
    UnkMeleeComponent* melee;
};

bool fn_7100d7b50c(UnkSluggerBallOther_b59a4* other);

struct UnkSluggerBallKey_b59a4;
extern UnkSluggerBallKey_b59a4 g_unk_710164a780;  // reached through the GOT

struct UnkSluggerBallHeld_b59a4 {
    u8 pad_0x0[0x10];
    UnkSluggerBallOther_b59a4* other;
};

struct UnkSluggerBall_b59a4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x230 - 0x38];
    UnkSluggerBallHeld_b59a4* field_0x230;
};

bool fn_71006c7ba4(UnkSluggerBall_b59a4* self, UnkSluggerBallOther_b59a4* other, void* a2, void* a3, void* a4,
                   void* a5) {
    if (!other)
        return false;
    UnkSluggerBallHeld_b59a4* held = self->field_0x230;
    if ((held ? held->other : nullptr) == other)
        return false;
    if (fn_7100d7b50c(other) && other->melee && fn_710092b604(other->melee)) {
        if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a780), &mask.id, false, -1.0f);
        }
        return true;
    }
    return fn_7100ac4ec0(reinterpret_cast<UnkObj*>(self), other, a2, a3, a4, a5);
}
