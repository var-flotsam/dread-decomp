// Unit 1452 (tools/tu_map.py): a step that sets its flag (+0xe0), looks up the owner's gun named by
// the id at +0x1240 of the reflection singleton (fn_710030c5fc) and caps two of its target's floats
// (+0x538, +0x540) at 350, then continues with the object's vf_0x70. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAbilityGhostAura.h"
#include "game/UnkTU1450.h"
#include "types.h"
#include <math.h>
#include "../../game_s7/UnkPropBagS7.h"
#include "base/math/CVector.h"
#include "game/UnkXformNode.h"

using base::global::CStrId;

struct UnkAuraGunOwner;
struct CGrappleBeamGun;
struct UnkUnit1452_b56;
struct UnkUnit1452Owner_b56;

struct UnkGunSlot_b0249 {
    u8 pad_0x0[0x10];
    char* target;  // 0x10
};

struct UnkGun_b0249 {
    u8 pad_0x0[0x1888];
    UnkGunSlot_b0249* slot;
};

struct UnkStep_b0249 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70(void* arg);
    u8 pad_0x8[0xd8];
    bool field_0xe0;
};
static_assert(__builtin_offsetof(UnkStep_b0249, field_0xe0) == 0xe0, "layout");

static inline const CStrId& Name_b0249() {
    return *reinterpret_cast<const CStrId*>(reinterpret_cast<const char*>(&base::reflection::refl_GetSingleton()) +
                                            0x1240);
}

void fn_7100981514(UnkStep_b0249* self, void* arg) {
    self->field_0xe0 = true;
    UnkAuraGunOwner* owner = *reinterpret_cast<UnkAuraGunOwner**>(reinterpret_cast<char*>(fn_710097f1b8(reinterpret_cast<UnkUnit1452_b56*>(self))) + 0x2a8);
    if (UnkGun_b0249* gun = reinterpret_cast<UnkGun_b0249*>(fn_710030c5fc(owner, Name_b0249()))) {
        UnkGunSlot_b0249* slot = gun->slot;
        char* t = reinterpret_cast<char*>(slot);
        if (slot)
            t = slot->target;
        f32 v = *reinterpret_cast<f32*>(t + 0x538);
        v = !(v >= 350.0f) ? v : 350.0f;
        *reinterpret_cast<f32*>(t + 0x538) = v;
        *reinterpret_cast<f32*>(t + 0x540) = v;
    }
    self->vf_0x70(arg);
}

// Unit 1452 (tools/tu_map.py): feeds an angle to the owner's property bag: the direction from the owner's
// position (the translation of its transform at +0x38) to the position the object (fn_710097f1e0, vf_0x188)
// gives, projected to 2D, normalised, and turned into degrees as atan2(y, |x|). Names and layouts are
// placeholders.


extern const base::global::CStrId g_unk_717273ae40 __attribute__((visibility("hidden")));

struct UnkUnit1452_b56;
struct UnkUnit1452Owner_b56;

struct UnkBagHolder_b0249 {
    u8 pad_0x0[0x80];
    unk_prop_bag::UnkPropBag bag;
};

struct UnkAimOwner_b0249 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x220 - 0x94];
    UnkBagHolder_b0249* holder;  // 0x220
};

struct UnkPositionSource_b0249;

struct UnkVec2f_b0249 {
    f32 x, y;
};

typedef UnkVec3f (*UnkPositionFn_b0249)(UnkPositionSource_b0249* self);

void fn_7100981048(UnkUnit1452_b56* self) {
    UnkAimOwner_b0249* owner = reinterpret_cast<UnkAimOwner_b0249*>(fn_710097f1b8(self));
    UnkBagHolder_b0249* holder = owner->holder;
    UnkPositionSource_b0249* source = fn_710097f1e0(self);
    base::math::CVector3D d;
    UnkVec3f pos = reinterpret_cast<UnkPositionFn_b0249*>(*reinterpret_cast<void***>(source))[0x188 / 8](source);
    f32 tx = owner->xform.m[0][3], ty = owner->xform.m[1][3], tz = owner->xform.m[2][3];
    d.x = pos.x - tx;
    d.y = pos.y - ty;
    d.z = pos.z - tz;
    base::math::CVector2D p;
    fn_710007b430(&p, &d);
    f32 len = sqrtf(p.x * p.x + p.y * p.y);
    f32 nx, ny;
    if (len != 0.0f) {
        f32 inv = 1.0f / len;
        nx = inv * p.x;
        ny = inv * p.y;
    } else {
        nx = p.x;
        ny = p.y;
    }
    f32 angle = atan2f(ny, fabsf(nx));
    unk_prop_bag::fn_710033187c(&holder->bag, g_unk_717273ae40, (f32)((angle * 180.0f) / 3.141592653589793));
}
