// CCentralUnitCannonBeamMovementComponent's unit (tools/tu_map.py): the beam's impact. Unless the hit is one the
// base ignores (an actor with an AI component whose +0x1b8 part is in state 2, while the flags at +0x3fc allow
// it), sets the "onimpact_<area>" event of the area kind at +0x5d8 on the owner's timeline to the all-flags
// mask; then runs the base impact (fn_7100ac4864). Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CName.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkProjectileHit_lac4864;
struct UnkHitActor_lac4864;

s32 fn_7100ac4864(UnkProjectileHit_lac4864* self, UnkHitActor_lac4864* other);

struct UnkCannonHitState_l4005d0 {
    u8 pad_0x0[0x70];
    s32 state;  // 0x70
};

struct UnkCannonHitActor_l4005d0 {
    u8 pad_0x0[0x1b8];
    UnkCannonHitState_l4005d0* field_0x1b8;
    u8 pad_0x1c0[0x80];
    void* ai;  // 0x240
};

struct UnkCannonBeamMovement_l4005d0 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x3c4];
    u32 flags;  // 0x3fc
    u8 pad_0x400[0x1d8];
    u32 area;  // 0x5d8
};
static_assert(__builtin_offsetof(UnkCannonBeamMovement_l4005d0, flags) == 0x3fc, "layout");
static_assert(__builtin_offsetof(UnkCannonBeamMovement_l4005d0, area) == 0x5d8, "layout");

// The impact event of an area kind; false for an unknown one.
static inline bool ImpactEvent(u32 area, u64* event) {
    switch (area) {
    case 0:
        *event = base::global::CName("onimpact_cave").hash;
        return true;
    case 1:
        *event = base::global::CName("onimpact_magma").hash;
        return true;
    case 2:
        *event = base::global::CName("onimpact_lab").hash;
        return true;
    case 3:
        *event = base::global::CName("onimpact_forest").hash;
        return true;
    case 4:
        *event = base::global::CName("onimpact_sanc").hash;
        return true;
    }
    return false;
}

s32 fn_71004005d0(UnkCannonBeamMovement_l4005d0* self, UnkCannonHitActor_l4005d0* other) {
    u32 flags = self->flags;
    bool checked = !(flags & 0xc) & (other != nullptr) & !(flags & 2);
    if (!(checked && other->ai && other->field_0x1b8 && other->field_0x1b8->state == 2)) {
        if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
            u64 event;
            if (ImpactEvent(self->area, &event)) {
                base::reflection::UnkReflTypeIdArg mask;
                mask.id = UnkAll_SpitSend().value;
                fn_7100a2cb7c(timeline, &event, &mask.id, false, -1.0f);
            }
        }
    }
    return fn_7100ac4864(reinterpret_cast<UnkProjectileHit_lac4864*>(self),
                         reinterpret_cast<UnkHitActor_lac4864*>(other));
}
