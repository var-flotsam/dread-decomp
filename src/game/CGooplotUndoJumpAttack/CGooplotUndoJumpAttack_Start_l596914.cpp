// CGooplotUndoJumpAttack unit (tools/tu_map.py): the undo jump's start. Records the owner's position and
// the AI component's jump direction (fn_710059b7ec), enters state 1, writes the signed angle in radians
// between the owner's up axis and that direction (clamped to a quarter turn either way) into the
// animation values, and lets a Gooshocker AI know (fn_710059f058). Names and layouts are placeholders.
#include <math.h>

#include "../../game_s7/UnkPropBagS7.h"
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkGooplotJump_ovb0123;
void fn_7100594d5c(UnkGooplotJump_ovb0123* self, u32 state);
using base::reflection::UnkReflObject;
void fn_710059f058(UnkReflObject* ai);
UnkVec3f fn_710059b7ec(void* ai);

// The string id of the angle value, reached PC-relative.
extern char g_unk_71726e7170[] __attribute__((visibility("hidden")));

struct UnkGooplotUndoJump_l596914 {
    void** vtable;
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x50];
    bool field_0x60;
    u8 pad_0x61[3];
    UnkVec3f dir;       // 0x64
    UnkVec3f position;  // 0x70
};

void fn_7100596914(UnkGooplotUndoJump_l596914* self) {
    UnkActor* owner = self->owner;
    self->field_0x60 = false;
    self->position = UnkVec3f(owner->field_0x44, owner->field_0x54, owner->field_0x64);
    self->dir = fn_710059b7ec(owner->field_0x240);
    fn_7100594d5c(reinterpret_cast<UnkGooplotJump_ovb0123*>(self), 1);

    const UnkMatrix44& up_m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
    f32 ux = up_m.m[1];
    f32 uy = up_m.m[5];
    f32 uz = up_m.m[9];
    const UnkMatrix44& fwd_m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
    f32 fx = fwd_m.m[2];
    f32 fy = fwd_m.m[6];
    f32 fz = fwd_m.m[10];

    const UnkVec3f& d = self->dir;
    f32 dot = ux * d.x + uy * d.y + uz * d.z;
    f32 cosine = dot / (sqrtf(ux * ux + uy * uy + uz * uz) * sqrtf(d.x * d.x + d.y * d.y + d.z * d.z));
    f32 angle;
    if (cosine >= 1.0f)
        angle = 0.0f;
    else if (cosine <= -1.0f)
        angle = 3.14159265f;
    else
        angle = acosf(cosine);

    f32 side = fx * d.x + fy * d.y + fz * d.z;
    s32 sign = (side < 0.0f) - (side > 0.0f);
    angle *= sign;
    const f32 limit = 1.57079633f;
    angle = !(angle <= -limit) ? (!(angle >= limit) ? angle : limit) : -limit;

    UnkAnimationComponent* anim = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    unk_prop_bag::fn_710033187c(reinterpret_cast<unk_prop_bag::UnkPropBag*>(anim->field_0x80),
                                *reinterpret_cast<const unk_prop_bag::CStrId*>(g_unk_71726e7170), angle);

    UnkReflObject* ai = static_cast<UnkReflObject*>(self->owner->field_0x240);
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_710059e014()))
        fn_710059f058(ai);
}
