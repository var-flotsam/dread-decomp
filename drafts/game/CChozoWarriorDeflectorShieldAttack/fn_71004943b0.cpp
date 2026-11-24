// CChozoWarriorDeflectorShieldAttack's unit (tools/tu_map.py): whether the attack may start. For a Chozo
// Warrior AI owner the base check must pass, the AI's flag at +0x14999 be set, the distance it reads
// (by facing) exceed the limit at +0x4c, the target lie ahead and the AI's fn_710049cbd8 be false.
// `arg` and `mode` reach the base untouched in the original. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVectorOps.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CChozoWarriorAttack.h"
#include "game/UnkActor.h"
#include "types.h"

using base::math::CVector3D;
using base::reflection::UnkReflObject;

bool fn_71010fc554(base::reflection::UnkReflObject* component, f32* out);
bool fn_71010fc5fc(base::reflection::UnkReflObject* component, f32* out);
// As src/game/CChozoWarriorAIComponent/fn_710049cbd8.cpp defines it.
struct UnkSelf;
bool CChozoWarriorAIComponent_IsTargetAboveMinDistanceToFloor(UnkSelf* self);

struct UnkDeflectorWarriorAI : UnkReflObject {
    u8 pad_0x8[0x14991];
    bool field_0x14999;
};
static_assert(__builtin_offsetof(UnkDeflectorWarriorAI, field_0x14999) == 0x14999, "layout");

struct UnkDeflectorAttack {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x3c];
    f32 field_0x4c;
};

static inline CVector3D Pos(const UnkActor* actor) {
    return {actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

static inline const UnkMatrix44& Matrix(UnkActor* actor) {
    return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
}

// The Chozo Warrior's own conditions.
static inline bool CanDeflect(UnkDeflectorAttack* self, UnkDeflectorWarriorAI* ai) {
    if (!ai->field_0x14999)
        return false;
    f32 distance = 0.0f;
    if (Matrix(self->owner).m[2] >= 0.0f) {
        fn_71010fc554(ai, &distance);
        if (!(distance > self->field_0x4c))
            return false;
    } else {
        fn_71010fc5fc(ai, &distance);
        if (!(distance > self->field_0x4c))
            return false;
    }
    UnkActor* target = fn_7100d7c990(self->owner);
    CVector3D d = Pos(target) - Pos(self->owner);
    f32 length = Length(d);
    if (!target)
        return false;
    f32 inv = 1.0f / (length + 0.0001f);
    d.z *= inv;
    d.y *= inv;
    d.x *= inv;
    const UnkMatrix44& m = Matrix(self->owner);
    f32 mx = m.m[2];
    f32 my = m.m[6];
    f32 mz = m.m[10];
    f32 dot = d.x * mx + d.y * my + d.z * mz;
    if (!(dot > 0.3f))
        return false;
    return !CChozoWarriorAIComponent_IsTargetAboveMinDistanceToFloor(reinterpret_cast<UnkSelf*>(ai));
}

bool fn_71004943b0(UnkDeflectorAttack* self, void* arg, u32 mode) {
    bool ok = fn_7100493b00(static_cast<void*>(self), arg, mode);
    UnkReflObject* component = static_cast<UnkReflObject*>(self->owner->field_0x240);
    if (component && fn_710009d50c(component->vf_0x0(), fn_7100497f2c())) {
        if (!ok)
            return false;
        return CanDeflect(self, static_cast<UnkDeflectorWarriorAI*>(component));
    }
    return ok;
}
