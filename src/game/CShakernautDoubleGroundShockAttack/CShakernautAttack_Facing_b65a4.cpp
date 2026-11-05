// The Shakernaut attacks' facing test, in two units with the same body (tools/tu_map.py:
// CShakernautDoubleGroundShockAttack fn_71006a5040, CShakernautPiercingLaserAttack fn_71006a7dc8):
// whether the target actor (fn_7100d7c990 of the owner at +0x8) lies ahead of the owner, i.e. the 2D
// direction from the owner to the target (normalised unless zero) has a positive dot product with the
// facing at +0x14810 of the owner's AI component (+0x240, cast to CShakernautAIComponent). Names and
// layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkShakernautAttack_b65a4 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
};

static inline CVector3D Translation_b65a4(const UnkActor* actor) {
    CVector3D v;
    v.x = actor->field_0x44;
    v.y = actor->field_0x54;
    v.z = actor->field_0x64;
    return v;
}

__attribute__((always_inline)) static inline bool FacesTarget_b65a4(UnkShakernautAttack_b65a4* self) {
    UnkActor* target = fn_7100d7c990(self->owner);
    CVector2D to;
    CVector3D target3 = Translation_b65a4(target);
    fn_710007b430(&to, &target3);
    CVector2D from;
    CVector3D from3 = Translation_b65a4(self->owner);
    fn_710007b430(&from, &from3);
    f32 dx = to.x - from.x;
    f32 dy = to.y - from.y;
    f32 len = sqrtf(dx * dx + dy * dy);
    if (len != 0.0f) {
        dx /= len;
        dy /= len;
    }
    base::reflection::UnkReflObject* ai = static_cast<base::reflection::UnkReflObject*>(self->owner->field_0x240);
    char* cast = fn_710009d50c(ai->vf_0x0(), fn_71006ad0a4()) ? reinterpret_cast<char*>(ai) : nullptr;
    const CVector2D* facing = reinterpret_cast<const CVector2D*>(cast + 0x14810);
    return dx * facing->x + dy * facing->y > 0.0f;
}

bool fn_71006a5040(UnkShakernautAttack_b65a4* self) {
    return FacesTarget_b65a4(self);
}

bool fn_71006a7dc8(UnkShakernautAttack_b65a4* self) {
    return FacesTarget_b65a4(self);
}
