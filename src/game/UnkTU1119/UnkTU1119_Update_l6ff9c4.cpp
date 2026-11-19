// Unit 1119 of tools/tu_map.py (no name hint): the step of unit 1118's large AI object. Takes the values its
// overrides (+0x12cb0) hold; while the override's followed object is active (and the owner's character
// class is not one that ignores following a path) runs the path follow, otherwise aims at the owner; then,
// when the owner's effect sink is in mode 3, sends it a kind-3 parameter block (vtable g_unk_7101a2e208).
// Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

template <class T>
struct UnkOpt_l6ff9c4 {
    T value;
    bool has;
};

// The block sent to the effect sink (vtable g_unk_7101a2e208, kind 3).
struct UnkSinkParams_l6ff9c4 {
    void** vtable;
    UnkOpt_l6ff9c4<u32> field_0x8;
    UnkOpt_l6ff9c4<u32> field_0x10;
    UnkOpt_l6ff9c4<CVector3D> field_0x18;
    UnkOpt_l6ff9c4<CVector3D> dir;  // 0x28
    u32 kind;                       // 0x38
    u8 field_0x3c;
    bool field_0x3d;
};
static_assert(__builtin_offsetof(UnkSinkParams_l6ff9c4, kind) == 0x38, "layout");

struct UnkFollowed_l6ff9c4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20();  // whether it is active
};

struct UnkPair_l6ff9c4 {
    u64 field_0x0;
    u64 field_0x8;
};

// Values that override the object's own (each with whether it is set).
struct UnkOverrides_l6ff9c4 {
    UnkOpt_l6ff9c4<CVector3D> field_0x0;
    UnkOpt_l6ff9c4<u32> field_0x10;
    UnkOpt_l6ff9c4<UnkPair_l6ff9c4> field_0x18;
    UnkOpt_l6ff9c4<UnkFollowed_l6ff9c4*> followed;  // 0x30
    u8 pad_0x40[0x10f - 0x40];
    UnkOpt_l6ff9c4<u8> field_0x10f;
    u8 pad_0x111[0x170 - 0x111];
    u64 field_0x170;
};
static_assert(__builtin_offsetof(UnkOverrides_l6ff9c4, field_0x10f) == 0x10f, "layout");
static_assert(__builtin_offsetof(UnkOverrides_l6ff9c4, field_0x170) == 0x170, "layout");

struct UnkTU1119_l6ff9c4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;   // 0x30
    CVector3D target;  // 0x38
    CVector3D field_0x44;
    u32 field_0x50;
    u8 pad_0x54[4];
    UnkPair_l6ff9c4 field_0x58;
    u8 pad_0x68[0xac - 0x68];
    s32 field_0xac;
    u8 pad_0xb0[0xc8 - 0xb0];
    bool field_0xc8;
    u8 pad_0xc9[0xd4 - 0xc9];
    CVector2D field_0xd4;
    u8 pad_0xdc[8];
    CVector2D field_0xe4;
    u8 pad_0xec[0xf8 - 0xec];
    u32 field_0xf8;
    u8 pad_0xfc[0x210 - 0xfc];
    bool field_0x210;
    u8 pad_0x211[0x12cb0 - 0x211];
    UnkOverrides_l6ff9c4 overrides;  // 0x12cb0
};
static_assert(__builtin_offsetof(UnkTU1119_l6ff9c4, field_0x210) == 0x210, "layout");
static_assert(__builtin_offsetof(UnkTU1119_l6ff9c4, overrides) == 0x12cb0, "layout");

struct UnkAIState_l6fe894;
struct UnkReadyObject_l6fe894;
struct UnkTU1118Stepper;
struct UnkNavPath_b66a4;
struct UnkNavKind2_b66a4;
UnkNavPath_b66a4* fn_71006fce10(UnkNavKind2_b66a4* obj);
void fn_71006fc7c4(UnkAIState_l6fe894* self, UnkReadyObject_l6fe894* ready);
bool fn_71006ff330(UnkTU1118Stepper* self);
bool fn_71006fda48(UnkTU1119_l6ff9c4* self);
void fn_71006fdba4(UnkTU1119_l6ff9c4* self);
void fn_71006ffce4(UnkTU1119_l6ff9c4* self, f32 dt, CVector2D* pos);
void fn_71007003b4(UnkTU1119_l6ff9c4* self, CVector2D* pos);

void fn_71006ff9c4(UnkTU1119_l6ff9c4* self, f32 dt) {
    UnkOverrides_l6ff9c4& overrides = self->overrides;
    if (overrides.field_0x0.has) {
        self->field_0x44.x = overrides.field_0x0.value.x;
        self->field_0x44.y = overrides.field_0x0.value.y;
        self->field_0x44.z = overrides.field_0x0.value.z;
    }
    if (overrides.field_0x10.has)
        self->field_0x50 = overrides.field_0x10.value;
    if (overrides.field_0x18.has)
        self->field_0x58 = overrides.field_0x18.value;
    UnkFollowed_l6ff9c4* followed = overrides.followed.has ? overrides.followed.value : nullptr;
    u8 stop = overrides.field_0x10f.has ? overrides.field_0x10f.value : 0;

    bool following = false;
    if (followed) {
        auto* anim = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
        if (followed->vf_0x20() && !(anim && fn_710117b348(anim, fn_7100d0a858())))
            following = true;
    }
    if (following) {
        if (self->field_0xac < 0 && !self->field_0x210)
            fn_71006fc7c4(reinterpret_cast<UnkAIState_l6fe894*>(self),
                          reinterpret_cast<UnkReadyObject_l6fe894*>(fn_71006fce10(reinterpret_cast<UnkNavKind2_b66a4*>(self))));
        if (stop && !self->field_0xc8 && fn_71006fda48(self)) {
            self->field_0xf8 = 0;
            self->field_0x50 = 0;
            overrides.field_0x170 = 0;
            fn_71006fdba4(self);
        } else {
            CVector2D now;
            CVector2D pos;
            {
                UnkActor* owner = self->owner;
                f32 px = owner->field_0x44;
                f32 py = owner->field_0x54;
                f32 pz = owner->field_0x64;
                CVector3D owner_pos = {px, py, pz};
                fn_710007b430(&pos, &owner_pos);
            }
            if (fn_71006fda48(self) && self->field_0xc8)
                fn_71006ff330(reinterpret_cast<UnkTU1118Stepper*>(self));
            {
                UnkActor* owner = self->owner;
                f32 px = owner->field_0x44;
                f32 py = owner->field_0x54;
                f32 pz = owner->field_0x64;
                CVector3D owner_pos = {px, py, pz};
                fn_710007b430(&now, &owner_pos);
            }
            f32 dx = self->field_0xd4.x - now.x;
            f32 dy = self->field_0xd4.y - now.y;
            self->field_0xe4.x = dx;
            self->field_0xe4.y = dy;
            fn_71006ffce4(self, dt, &pos);
            fn_71007003b4(self, &pos);
        }
    } else {
        UnkActor* owner = self->owner;
        f32 px = owner->field_0x44;
        f32 py = owner->field_0x54;
        f32 pz = owner->field_0x64;
        self->target.x = px;
        self->target.y = py;
        self->target.z = pz;
    }

    if (!self->owner->field_0x218 || fn_71010cc3a4(self->owner->field_0x218) != 3)
        return;
    UnkActor* owner = self->owner;
    CVector2D from;
    {
        f32 px = owner->field_0x44;
        f32 py = owner->field_0x54;
        f32 pz = owner->field_0x64;
        CVector3D owner_pos = {px, py, pz};
        fn_710007b430(&from, &owner_pos);
    }
    CVector2D to;
    fn_710007b430(&to, &self->target);
    f32 x = to.x - from.x;
    f32 y = to.y - from.y;
    f32 length = sqrtf(x * x + y * y);
    if (length != 0.0f) {
        f32 inv = 1.0f / length;
        x *= inv;
        y *= inv;
    }

    UnkSinkParams_l6ff9c4 params;
    params.field_0x10.has = false;
    params.vtable = g_unk_7101a2e208 + 2;
    params.field_0x3d = false;
    params.dir.value.x = x;
    params.dir.value.y = y;
    params.kind = 3;
    params.dir.value.z = 0.0f;
    params.dir.has = true;
    params.field_0x18.value.x = self->field_0x44.x;
    params.field_0x18.value.y = self->field_0x44.y;
    params.field_0x18.value.z = self->field_0x44.z;
    params.field_0x18.has = true;
    params.field_0x8.value = self->field_0x50;
    params.field_0x8.has = true;
    fn_71010c7e6c(reinterpret_cast<UnkEffectSink*>(self->owner->field_0x218), &params, 1);
}
