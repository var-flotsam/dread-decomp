// UnkTU2702 (CAIComponent's unit): whether the owner and the actor it acts against face away from each
// other: true while the target plays a CCharClassMelee360ActionTrack, else by their facing signs.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAIComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkXformNode.h"
#include "types.h"

// The owner's slot at +0x218: vtable slot 0x370 gives its facing direction.
struct UnkFacing_l10c85c {
    void** vtable;

    UnkVec3f GetFacing() {
        return reinterpret_cast<UnkVec3f (*)(UnkFacing_l10c85c*)>(vtable[0x370 / 8])(this);
    }
};

static inline UnkXform* XformOf(UnkActor* actor) {
    return reinterpret_cast<UnkXform*>(actor->field_0x38);
}

inline f32 Sign(f32 v) {
    return (f32)((v > 0.0f) - (v < 0.0f));
}

bool fn_710110c85c(UnkAIComponent* self) {
    UnkActor* target = fn_7100d7c990(self->field_0x30);
    if (!target)
        return false;
    UnkAnimationComponent* anim = static_cast<UnkAnimationComponent*>(target->field_0x220);
    if (anim && fn_710117b348(anim, fn_7100c3d888()))
        return true;
    UnkActor* owner = self->field_0x30;
    UnkFacing_l10c85c* facing = static_cast<UnkFacing_l10c85c*>(owner->field_0x218);
    if (facing) {
        f32 ownerSign = Sign(facing->GetFacing().x);
        return ownerSign != Sign(XformOf(target)->GetMatrix().m[2]);
    }
    if (XformOf(owner)->GetAngle().y >= 0.0f && XformOf(target)->GetAngle().y <= 0.0f)
        return true;
    return XformOf(self->field_0x30)->GetAngle().y <= 0.0f && XformOf(target)->GetAngle().y >= 0.0f;
}
