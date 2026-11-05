// CKraidBouncingCreaturesMovementComponent's unit (tools/tu_map.py): the hit test. A null `other` is no
// hit; one whose movement (+0x1f0) is a CProjectileMovement hits at once. The one held at +0x230
// (+0x10) sets the id g_unk_710164a290 on the owner's (+0x30) timeline (+0x2e8) to the all-flags mask
// (as src/game/CKraidAIComponent/fn_7100600c98.cpp); any other one either passes fn_7100605790 with a4 (when it
// has no part at +0x240 or +0x200) or must not be playing a CCharClassMorphBallLauncherMovementTrack,
// and then must pass the base test fn_7100ac4ec0. A hit through the held one or the base test clears
// +0x4b8, +0x4c0 and +0x108. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkObj;
bool fn_7100ac4ec0(UnkObj* self, void* a1, void* a2, void* a3, void* a4, void* a5);

struct UnkBouncingKey_b59a4;
extern UnkBouncingKey_b59a4 g_unk_710164a290;  // reached through the GOT

struct UnkBouncingOther_b59a4 {
    u8 pad_0x0[0x1f0];
    base::reflection::UnkReflObject* movement;
    u8 pad_0x1f8[8];
    void* field_0x200;
    u8 pad_0x208[0x18];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    void* field_0x240;
};

struct UnkBouncingHeld_b59a4 {
    u8 pad_0x0[0x10];
    UnkBouncingOther_b59a4* other;
};

struct UnkBouncing_b59a4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x108 - 0x38];
    u32 field_0x108;
    u8 pad_0x10c[0x230 - 0x10c];
    UnkBouncingHeld_b59a4* field_0x230;
    u8 pad_0x238[0x4b8 - 0x238];
    u64 field_0x4b8;
    u32 field_0x4c0;
};

// The two middle arguments are not used: empty values whose registers the caller leaves as they are.
struct UnkUnused_b59a4 {};
bool fn_7100605790(UnkBouncing_b59a4* self, UnkUnused_b59a4, UnkUnused_b59a4, void* a4);

bool fn_7100605d64(UnkBouncing_b59a4* self, UnkBouncingOther_b59a4* other, void* a2, void* a3, void* a4,
                   void* a5) {
    if (!other)
        return false;
    if (other->movement && fn_710009d50c(other->movement->vf_0x0(), fn_7100ac2434()))
        return true;
    UnkBouncingHeld_b59a4* held = self->field_0x230;
    if ((held ? held->other : nullptr) == other) {
        if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a290), &mask.id, false, -1.0f);
        }
    } else {
        if (!other->field_0x240 && !other->field_0x200) {
            if (fn_7100605790(self, UnkUnused_b59a4(), UnkUnused_b59a4(), a4))
                return true;
        } else if (fn_710117b348(other->animation, fn_7100c4041c())) {
            return false;
        }
        if (!fn_7100ac4ec0(reinterpret_cast<UnkObj*>(self), other, a2, a3, a4, a5))
            return false;
    }
    self->field_0x4b8 = 0;
    self->field_0x4c0 = 0;
    self->field_0x108 = 0;
    return true;
}
