// CScorpiusAttack's unit: fn_7100686860 tests whether the owner's x position, measured from the Scorpius
// AI's reference limit (vf_0x5e0), is within the reach the attack's component reports (vf_0x30),
// when the owner is facing the limit. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkScorpiusAI_l686860 : UnkReflObject {
    void vf_0x5e0(f32* a, f32* b) {
        reinterpret_cast<void (*)(UnkScorpiusAI_l686860*, f32*, f32*)>((*reinterpret_cast<void***>(this))[0x5e0 / 8])(this, a, b);
    }
};

struct UnkScorpiusReach_l686860 {
    f32 vf_0x30(UnkActor* actor) {
        return reinterpret_cast<f32 (*)(UnkScorpiusReach_l686860*, UnkActor*)>((*reinterpret_cast<void***>(this))[0x30 / 8])(this, actor);
    }
};

struct UnkScorpiusAttack_l686860 {
    u8 pad_0x0[8];
    UnkActor* actor;  // 0x8
    u8 pad_0x10[0x18];
    UnkScorpiusReach_l686860* reach;  // 0x28
};

template <class T>
static inline T* Cast(UnkReflObject* object, const base::reflection::CClass* (*getter)()) {
    if (!object)
        return nullptr;
    return fn_710009d50c(object->vf_0x0(), getter()) ? static_cast<T*>(object) : nullptr;
}

bool fn_7100686860(UnkScorpiusAttack_l686860* self) {
    auto* ai = Cast<UnkScorpiusAI_l686860>(static_cast<UnkReflObject*>(self->actor->field_0x240), fn_7100690c74);
    f32 limit[2];
    __builtin_memset(limit, 0, 8);
    ai->vf_0x5e0(&limit[1], &limit[0]);
    f32 x = self->actor->field_0x44;
    f32 facing = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(&self->actor->field_0x38)).m[2];
    if (!(facing <= 0.0f && limit[0] <= x)) {
        if (!(x <= limit[0]))
            return false;
        asm("" : "+w"(facing));
        if (!(facing > 0.0f))
            return false;
    }
    f32 dist = fn_7100d7c990(self->actor)->field_0x44 - limit[0];
    UnkScorpiusReach_l686860* reach = self->reach;
    UnkActor* actor = self->actor;
    f32 signed_dist = limit[1] > limit[0] ? -dist : dist;
    return signed_dist <= reach->vf_0x30(actor);
}
