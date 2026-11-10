// CChozoWarriorGlaiveWalljumpAttack's unit (tools/tu_map.py unit 731): whether the attack may start. Besides the base check
// the owner's life must be positive and, for a Chozo Warrior AI, it must be far enough from the reach, facing the
// walls and not in the way of the target. `arg` and `mode` reach the base untouched in the original. Names and layouts
// are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::reflection::UnkReflObject;

extern "C" f32 sqrtf(f32);

struct UnkWalljumpLife {
    u8 pad_0x0[0x54];
    f32 field_0x54;
};

struct UnkWalljumpAttack {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
};

struct UnkWarriorAI {
    u8 pad_0x0[8];
};

bool fn_7100493b00(void* self, void* arg, u32 mode);
f32 fn_710049b2fc(UnkWarriorAI* ai);
f32 fn_710049b360(UnkWarriorAI* ai);
bool fn_710049cbd8(UnkWarriorAI* ai);
bool fn_7100df5630(UnkGameManager* manager);
bool fn_7100df5a4c(UnkGameManager* manager);

struct UnkV3s_ovb0265 {
    f32 x, y, z;
};

static inline UnkV3s_ovb0265 operator-(const UnkV3s_ovb0265& a, const UnkV3s_ovb0265& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline UnkV3s_ovb0265 Pos(const UnkActor* actor) {
    return {actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

static inline const UnkMatrix44& Matrix(UnkActor* actor) {
    return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
}

bool fn_7100495740(UnkWalljumpAttack* self, void* arg, u32 mode) {
    if (!fn_7100493b00(static_cast<void*>(self), arg, mode))
        return false;
    UnkActor* owner = self->owner;
    if (UnkWalljumpLife* life = static_cast<UnkWalljumpLife*>(owner->field_0x1b8)) {
        if (!(life->field_0x54 > 0.0f))
            return false;
    }
    UnkReflObject* component = static_cast<UnkReflObject*>(owner->field_0x240);
    if (!component)
        return true;
    if (!fn_710009d50c(component->vf_0x0(), fn_7100497f2c()))
        return true;
    UnkWarriorAI* ai = reinterpret_cast<UnkWarriorAI*>(component);
    f32 reach;
    if (Matrix(self->owner).m[2] >= 0.0f)
        reach = fn_710049b360(ai);
    else
        reach = fn_710049b2fc(ai);
    u32 result = 0;
    if (!(__builtin_fabsf(self->owner->field_0x44 - reach) > 100.0f))
        goto done;
    if (__builtin_fabsf(Matrix(self->owner).m[2]) < 0.99f)
        goto done;
    if (fn_710049cbd8(ai)) {
        bool in_cutscene = fn_7100df5630(g_unk_7101d47310);
        result = !in_cutscene;
        if (in_cutscene)
            goto done;
    } else {
        UnkActor* target = fn_7100d7c990(self->owner);
        UnkV3s_ovb0265 d = Pos(target) - Pos(self->owner);
        f32 length = sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);
        if (!target)
            goto done;
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
            goto done;
        result = 1;
    }
    if (fn_7100df5a4c(g_unk_7101d47310))
        result = 0;
done:
    return result != 0;
}
