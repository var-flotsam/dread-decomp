// CNailongAIComponent's unit (tools/tu_map.py 937): after the AI component's fn_7101105c94, when `actor` is
// the owner's (+0x30) actor (fn_7100d7c990) and the owner's animation component (+0x220) plays none of
// four track classes (fn_710117b348), sends the animation component the hit track for where the actor
// is: in front, behind, above or below the owner (mode 0, UnkSpitSend). Names are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/math/CVectorOps.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2702.h"
#include "types.h"

using base::math::CVector3D;

struct UnkWarLotusAI;

// An actor's position: the translation column of its transform.
static inline CVector3D Pos(const UnkActor* actor) {
    return {actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

struct UnkNailongHitOwner_l621adc {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

struct UnkNailongHit_l621adc {
    u8 pad_0x0[0x30];
    UnkNailongHitOwner_l621adc* owner;

    UnkActor* Actor() { return reinterpret_cast<UnkActor*>(owner); }
};

void fn_7100621adc(UnkNailongHit_l621adc* self, UnkActor* actor) {
    fn_7101105c94(reinterpret_cast<UnkWarLotusAI*>(self));
    UnkAnimationComponent* animation = self->owner ? self->owner->animation : nullptr;
    if (!actor)
        return;
    if (fn_7100d7c990(self->Actor()) != actor)
        return;
    if (animation) {
        if (fn_710117b348(animation, fn_7100d2b3a8()))
            return;
        if (fn_710117b348(animation, fn_7100cee4b8()))
            return;
        if (fn_710117b348(animation, fn_7100ceda9c()))
            return;
        if (fn_710117b348(animation, fn_7100d253f8()))
            return;
    }

    UnkActor* other = fn_7100d7c990(self->Actor());
    UnkActor* owner = self->Actor();
    CVector3D other_pos = Pos(other);
    CVector3D owner_pos = Pos(owner);
    CVector3D d = other_pos - owner_pos;
    // How far the actor is along the owner's facing (its z axis).
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38));
    CVector3D facing = {m.m[2], m.m[6], m.m[10]};
    f32 ahead = Dot(d, facing);
    u64 track;
    if (ahead > 0.7f)
        track = 0x229f1ce86d703ce5;
    else if (ahead < -0.7f)
        track = 0xd897a70729369c78;
    else if (owner_pos.y < other_pos.y)
        track = 0xfbdb07f3edcf090e;
    else
        track = 0x5588c587a9b3c8f4;
    UnkSpitSend(animation, track, 0xf4061ff25a2163d6);
}
