// CChozoCommanderKiCounterAttack's unit (tools/tu_map.py unit 651): fn_710042eea8, the counter position, as
// the Ki strike's (src/game/CChozoCommanderKiStrikeAttack/CChozoCommanderKiStrikeAttack_Position_ovb0114.cpp)
// without its distance offset. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::reflection::UnkReflObject;

bool fn_71010fc778(void*, f32*);
bool fn_71010fc39c(void*, f32*);

struct UnkKiCounterAI_ovb0136 {
    u8 pad_0x0[0x15ed0];
    bool field_0x15ed0;
};

struct UnkKiCounterDef_ovb0136 {
    u8 pad_0x0[0xf4];
    f32 minDistance;  // 0xf4
    f32 maxDistance;  // 0xf8
};

struct UnkKiCounter_ovb0136 {
    void* vtable;
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x40];
    UnkKiCounterDef_ovb0136* def;  // 0x50
};

struct UnkKiCounterVec_ovb0136 {
    f32 x, y, z;
    UnkKiCounterVec_ovb0136 operator*(f32 s) const { return {x * s, y * s, z * s}; }
};

static inline f32 Sign(f32 v) {
    return (f32)((v > 0.0f) - (v < 0.0f));
}

bool fn_710042eea8(UnkKiCounter_ovb0136* self, UnkVec3f* out) {
    UnkReflObject* obj = static_cast<UnkReflObject*>(self->owner->field_0x240);
    if (!obj || !fn_710009d50c(obj->vf_0x0(), fn_710044e708()))
        return false;
    if (reinterpret_cast<UnkKiCounterAI_ovb0136*>(obj)->field_0x15ed0)
        return false;
    if (!(self->def->minDistance > 0.0f))
        return false;
    UnkActor* owner = self->owner;
    *out = UnkVec3f(owner->field_0x44, owner->field_0x54, owner->field_0x64);
    f32 distance = 0.0f;
    f32 x = 0.0f;
    fn_71010fc778(obj, &distance);
    fn_71010fc39c(obj, &x);
    UnkKiCounterDef_ovb0136* def = self->def;
    if (distance >= def->minDistance) {
        if (distance > def->maxDistance)
            distance = def->maxDistance;
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
        UnkKiCounterVec_ovb0136 back = UnkKiCounterVec_ovb0136{m.m[2], m.m[6], m.m[10]} * distance;
        out->x -= back.x;
        out->y -= back.y;
        out->z -= back.z;
        return true;
    }
    UnkActor* target = fn_7100d7c990(self->owner);
    f32 xv = x;
    f32 targetX = target->field_0x44;
    f32 d = __builtin_fabsf(xv - targetX);
    def = self->def;
    if (!(d >= def->minDistance))
        return false;
    d = d > def->maxDistance ? def->maxDistance : d;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
    out->x = targetX + d * Sign(m.m[2]);
    return true;
}
