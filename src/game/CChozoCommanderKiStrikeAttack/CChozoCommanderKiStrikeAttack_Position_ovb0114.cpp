// CChozoCommanderKiStrikeAttack's unit (tools/tu_map.py): the strike position, from the Chozo Commander AI
// component's distances (fn_71010fc778, fn_71010fc39c) and the definition's range (+0xf4, +0xf8). Names
// and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::reflection::UnkReflObject;

bool fn_71010fc778(void*, f32*);
bool fn_71010fc39c(void*, f32*);

struct UnkKiStrikeAI3_ovb0114 {
    u8 pad_0x0[0x15ed0];
    bool field_0x15ed0;
};

struct UnkKiStrikeDef_ovb0114 {
    u8 pad_0x0[0xf4];
    f32 minDistance;  // 0xf4
    f32 maxDistance;  // 0xf8
};

struct UnkKiStrike3_ovb0114 {
    void* vtable;
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x40];
    UnkKiStrikeDef_ovb0114* def;  // 0x50
    u8 pad_0x58[0x10];
    f32 field_0x68;
};

struct Vec3_ovb0114 {
    f32 x, y, z;
    Vec3_ovb0114 operator*(f32 s) const { return {x * s, y * s, z * s}; }
    Vec3_ovb0114 operator-(const Vec3_ovb0114& o) const { return {x - o.x, y - o.y, z - o.z}; }
};

static inline f32 Sign(f32 v) {
    return (f32)((v > 0.0f) - (v < 0.0f));
}

// The strike position: from the owner, backed off along its facing by the AI component's distance
// (fn_71010fc778) while that is under the definition's minimum; otherwise at the target's x side
// distance (from fn_71010fc39c's x), when that reaches the minimum. False when there is none.
bool fn_71004369b4(UnkKiStrike3_ovb0114* self, UnkVec3f* out) {
    UnkReflObject* obj = static_cast<UnkReflObject*>(self->owner->field_0x240);
    if (!obj || !fn_710009d50c(obj->vf_0x0(), fn_710044e708()))
        return false;
    if (reinterpret_cast<UnkKiStrikeAI3_ovb0114*>(obj)->field_0x15ed0)
        return false;
    if (!(self->def->minDistance > 0.0f))
        return false;
    UnkActor* owner = self->owner;
    *out = UnkVec3f(owner->field_0x44, owner->field_0x54, owner->field_0x64);
    f32 distance = 0.0f;
    f32 x = 0.0f;
    fn_71010fc778(obj, &distance);
    distance -= self->field_0x68;
    fn_71010fc39c(obj, &x);
    UnkKiStrikeDef_ovb0114* def = self->def;
    if (distance >= def->minDistance) {
        if (distance > def->maxDistance)
            distance = def->maxDistance;
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
        Vec3_ovb0114 back = Vec3_ovb0114{m.m[2], m.m[6], m.m[10]} * distance;
        out->x -= back.x;
        out->y -= back.y;
        out->z -= back.z;
        return true;
    }
    UnkActor* target = fn_7100d7c990(self->owner);
    f32 xv = x;
    f32 targetX = target->field_0x44;
    f32 d = __builtin_fabsf(xv - targetX) - self->field_0x68;
    def = self->def;
    if (!(d >= def->minDistance))
        return false;
    d = d > def->maxDistance ? def->maxDistance : d;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
    out->x = targetX + d * Sign(m.m[2]);
    return true;
}
