// CCharClassScorpiusAttack's unit (tools/tu_map.py unit 1007): fn_710067e048 lowers `*distance` to the
// actor's target's horizontal gap from the Scorpius AI's reference point (vf_0x5e0), less 20, the target's
// body reach (fn_7100977b20) and the char class's reach (+0x124), when the actor is within that reach plus
// 20 of the point. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCharacterMovement.h"
#include "game/UnkActor.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkScorpiusCharClass_ovb0125 : UnkReflObject {
    u8 pad_0x8[0x11c];
    f32 reach;  // 0x124
};

struct UnkScorpiusAI_ovb0125 : UnkReflObject {
    u8 pad_0x8[0x1608];
    UnkScorpiusCharClass_ovb0125* charclass;  // 0x1610

    void vf_0x5e0(f32* x, f32* y) {
        reinterpret_cast<void (*)(UnkScorpiusAI_ovb0125*, f32*, f32*)>((*reinterpret_cast<void***>(this))[0x5e0 / 8])(this, x, y);
    }
};

// The actor's body as this file reads it.
struct UnkScorpiusBody_ovb0125 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0x1b8];
    void* field_0x200;
};

template <class T>
static inline T* Cast(UnkReflObject* object, const base::reflection::CClass* (*getter)()) {
    return fn_710009d50c(object->vf_0x0(), getter()) ? static_cast<T*>(object) : nullptr;
}

void fn_710067e048(void* self, UnkActor* actor, f32* distance) {
    auto* ai = Cast<UnkScorpiusAI_ovb0125>(static_cast<UnkReflObject*>(actor->field_0x240), fn_7100690c74);
    UnkScorpiusCharClass_ovb0125* cc = ai->charclass;
    if (cc)
        cc = Cast<UnkScorpiusCharClass_ovb0125>(cc, fn_710068f63c);
    f32 x = 0.0f;
    f32 y = 0.0f;
    ai->vf_0x5e0(&x, &y);
    if (__builtin_fabsf(x - actor->field_0x44) < cc->reach + 20.0f) {
        auto* target = reinterpret_cast<UnkScorpiusBody_ovb0125*>(fn_7100d7c990(actor));
        if (target && target->field_0x200) {
            f32 d = __builtin_fabsf(target->x - x) - 20.0f;
            f32 v = d - fn_7100977b20(target->field_0x200) - cc->reach;
            *distance = *distance < v ? *distance : v;
        }
    }
}
