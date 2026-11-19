// CChozoWarriorGlaiveSpinAttack's unit (tools/tu_map.py unit 730): whether the spin attack may start. Besides the
// base check the owner must face a wall (its forward x near +-1) with the target ahead of it, and, for a Chozo
// Warrior AI, fn_710049cbd8 must not hold. `arg` and `mode` reach the base untouched in the original. Names and
// layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/math/CVectorOps.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CChozoWarriorAttack.h"
#include "game/UnkActor.h"
#include "types.h"

using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkGlaiveSpinAttack {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
};

struct UnkWarriorAI {
    u8 pad_0x0[8];
};

bool fn_710049cbd8(UnkWarriorAI* ai);

static inline CVector3D Pos(const UnkActor* actor) {
    return {actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

static inline const UnkMatrix44& Matrix(UnkActor* actor) {
    return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
}

u32 fn_7100494c9c(UnkGlaiveSpinAttack* self, void* arg, u32 mode) {
    if (!fn_7100493b00(static_cast<void*>(self), arg, mode))
        return 0;
    if (__builtin_fabsf(Matrix(self->owner).m[2]) < 0.99f)
        return 0;
    UnkActor* target = fn_7100d7c990(self->owner);
    CVector3D d = Pos(target) - Pos(self->owner);
    f32 length = Length(d);
    if (!target)
        return 0;
    f32 inv = 1.0f / (length + 0.0001f);
    d.z *= inv;
    d.y *= inv;
    d.x *= inv;
    const UnkMatrix44& m = Matrix(self->owner);
    f32 mx = m.m[2];
    f32 my = m.m[6];
    f32 mz = m.m[10];
    f32 dot = d.x * mx + d.y * my + d.z * mz;
    if (!(dot >= 0.0f))
        return 0;
    UnkReflObject* component = static_cast<UnkReflObject*>(self->owner->field_0x240);
    if (component && fn_710009d50c(component->vf_0x0(), fn_7100497f2c()))
        return !fn_710049cbd8(reinterpret_cast<UnkWarriorAI*>(component));
    return 1;
}
