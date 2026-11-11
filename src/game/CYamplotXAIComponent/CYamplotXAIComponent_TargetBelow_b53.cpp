// CYamplotXAIComponent's unit (tools/tu_map.py): whether the target's position (fn_7100d7c990 of the
// actor), projected to 2D (fn_710007b430), is below the limit at +0x90 of the actor's movement
// component (+0x1f0, cast to CEnemyMovement; its +0x8e8 when fn_71010cc3a4 answers 1). Names and
// layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkYamplotLimits_b53 {
    u8 pad_0x0[0x90];
    f32 field_0x90;
};

struct UnkYamplotXAIComponent_b53 {
    u8 pad_0x0[0x30];
    UnkActor* actor;
};

// The actor's position (the translation column of its transform), read as one value.
static inline CVector3D GetPosition_b53(const UnkActor* actor) {
    f32 x = actor->field_0x44, y = actor->field_0x54, z = actor->field_0x64;
    return CVector3D{x, y, z};
}

static inline UnkReflObject* GetMovement_b53(UnkActor* actor) {
    return *reinterpret_cast<UnkReflObject**>(reinterpret_cast<char*>(actor) + 0x1f0);
}

bool fn_71006f230c(UnkYamplotXAIComponent_b53* self) {
    UnkActor* target = fn_7100d7c990(self->actor);
    UnkReflObject* movement = GetMovement_b53(self->actor);
    if (movement && !fn_710009d50c(movement->vf_0x0(), fn_71010c74e0()))
        movement = nullptr;
    UnkYamplotLimits_b53* limits = fn_71010cc3a4(movement) == 1
        ? reinterpret_cast<UnkYamplotLimits_b53*>(reinterpret_cast<char*>(movement) + 0x8e8)
        : nullptr;
    CVector3D pos = GetPosition_b53(target);
    CVector2D projected;
    fn_710007b430(&projected, &pos);
    f32 limit = limits->field_0x90;
    return projected.y < limit;
}
