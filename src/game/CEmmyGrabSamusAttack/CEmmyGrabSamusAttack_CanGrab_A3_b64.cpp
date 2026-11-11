// CEmmyGrabSamusAttack's unit (tools/tu_map.py): whether the grab may start. Only in mode 1 (+0x38's
// +0x10); true when there is no target (fn_7100d7c990) or the actor (+0x8) has no CEmmyAIComponent.
// When the EMMI is busy (its part +0x29418 active, or flag +0x29f26) and its track 0xf4061ff25a2163d6 is
// on (fn_7101179c3c), or its animation plays the CCharClassEmmyFloorSlideAllowedTrack class, a target
// in state 3 (+0x208's +0x2d08 +0x10) refuses; then neither the target's nor the actor's entity state
// (+0x1b8, kind +0x70) may be 2. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU39.h"
#include "types.h"

bool fn_7101179c3c(UnkAnimationComponent* self, u64 key);

struct UnkGrabKind2_A3_b64 {
    u8 pad_0x0[0x10];
    u32 field_0x10;
};

struct UnkGrabEntityState_A3_b64 {
    u8 pad_0x0[0x70];
    u32 kind;
};

struct UnkGrabTargetPart_A3_b64 {
    u8 pad_0x0[0x2d08];
    UnkGrabKind2_A3_b64* field_0x2d08;
};

struct UnkGrabCanAttack_A3_b64 {
    u8 pad_0x0[0x8];
    UnkActor* actor;
    u8 pad_0x10[0x28];
    UnkGrabKind2_A3_b64* field_0x38;
};

static inline UnkGrabEntityState_A3_b64* EntityState_A3_b64(UnkActor* a) {
    return static_cast<UnkGrabEntityState_A3_b64*>(a->field_0x1b8);
}

bool fn_71005061d4(UnkGrabCanAttack_A3_b64* self) {
    if (!self->field_0x38 || self->field_0x38->field_0x10 != 1)
        return false;
    UnkActor* target = fn_7100d7c990(self->actor);
    if (!target)
        return true;
    base::reflection::UnkReflObject* ai = static_cast<base::reflection::UnkReflObject*>(self->actor->field_0x240);
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_710053b934()))
        return true;
    char* emmy = reinterpret_cast<char*>(ai);
    bool track;
    if (fn_710002fb70(emmy + 0x29418) || *reinterpret_cast<bool*>(emmy + 0x29f26)) {
        UnkAnimationComponent* animation = static_cast<UnkAnimationComponent*>(self->actor->field_0x220);
        track = animation ? fn_7101179c3c(animation, 0xf4061ff25a2163d6) : false;
    } else {
        track = false;
    }
    UnkAnimationComponent* animation = static_cast<UnkAnimationComponent*>(self->actor->field_0x220);
    bool sliding = animation ? fn_710117b348(animation, fn_7100cf4e68()) : false;
    if (track || sliding) {
        UnkGrabTargetPart_A3_b64* part = static_cast<UnkGrabTargetPart_A3_b64*>(target->field_0x208);
        if (part && part->field_0x2d08 && part->field_0x2d08->field_0x10 == 3)
            return false;
    }
    if (EntityState_A3_b64(target) && EntityState_A3_b64(target)->kind == 2)
        return false;
    UnkGrabEntityState_A3_b64* state = EntityState_A3_b64(self->actor);
    return state ? state->kind != 2 : true;
}
