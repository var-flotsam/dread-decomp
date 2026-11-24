// CScourgeAIComponent's unit (tools/tu_map.py, unit 1035): after the base update, when `target` is the
// actor's current target, writes the signed angle in degrees between the actor's forward axis and the
// offset to the target into the animation component's values. Names and layouts are placeholders.
#include <math.h>

#include "../../game_s7/UnkPropBagS7.h"
#include "base/UnkMatrix44.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkScourgeAI_l6a0620 {
    u8 pad_0x0[0x30];
    UnkActor* actor;  // 0x30
};

// The string id of the angle value, reached PC-relative.
extern char g_unk_7172704f30[] __attribute__((visibility("hidden")));

void CScourgeAIComponent_StoreBiteAngle(UnkScourgeAI_l6a0620* self, UnkActor* target) {
    fn_7101105c94(reinterpret_cast<UnkWarLotusAI*>(self));
    UnkActor* current = fn_7100d7c990(self->actor);
    if (!current || current != target)
        return;

    UnkActor* actor = self->actor;
    f32 tx = target->field_0x44;
    f32 ty = target->field_0x54;
    f32 tz = target->field_0x64;
    f32 ox = actor->field_0x44;
    f32 oy = actor->field_0x54;
    f32 oz = actor->field_0x64;
    f32 dx = tx - ox;
    f32 dy = ty - oy;
    f32 dz = tz - oz;

    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
    f32 fx = m.m[2];
    f32 fy = m.m[6];
    f32 fz = m.m[10];
    f32 dot = dx * fx + dy * fy + dz * fz;
    f32 cosine = dot / (sqrtf(dx * dx + dy * dy + dz * dz) * sqrtf(fx * fx + fy * fy + fz * fz));
    f32 angle;
    if (cosine >= 1.0f)
        angle = 0.0f;
    else if (cosine <= -1.0f)
        angle = 180.0f;
    else
        angle = acosf(cosine) * 57.29578f;

    const UnkMatrix44& side = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->actor->field_0x38));
    UnkAnimationComponent* anim = static_cast<UnkAnimationComponent*>(self->actor->field_0x220);
    if (!anim)
        return;
    f32 across = dx * side.m[1] + dy * side.m[5] + dz * side.m[9];
    unk_prop_bag::fn_710033187c(reinterpret_cast<unk_prop_bag::UnkPropBag*>(anim->field_0x80),
                                *reinterpret_cast<const unk_prop_bag::CStrId*>(g_unk_7172704f30),
                                across < 0.0f ? -angle : angle);
}
