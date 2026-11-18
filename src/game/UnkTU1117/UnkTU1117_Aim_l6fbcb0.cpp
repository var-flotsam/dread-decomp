// Unit 1117 of tools/tu_map.py (no name hint): after the base step and the object's slot 0xa0, when the
// owner's effect sink (+0x218) is in mode 3, sends it a kind-3 parameter block (vtable g_unk_7101a2e208)
// with the values at +0x50 and +0x11c, the vector at +0x44 and the flat direction from the owner to the
// point at +0x38. Names and layouts are placeholders.
#include <math.h>
#include "base/math/CVector.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

template <class T>
struct UnkOpt_l6fbcb0 {
    T value;
    bool has;
};

// The block sent to the effect sink (vtable g_unk_7101a2e208, kind 3).
struct UnkSinkParams_l6fbcb0 {
    void** vtable;
    UnkOpt_l6fbcb0<u32> field_0x8;
    UnkOpt_l6fbcb0<u32> field_0x10;
    UnkOpt_l6fbcb0<CVector3D> field_0x18;
    UnkOpt_l6fbcb0<CVector3D> dir;  // 0x28
    u32 kind;                       // 0x38
    u8 field_0x3c;
    bool field_0x3d;
};
static_assert(__builtin_offsetof(UnkSinkParams_l6fbcb0, kind) == 0x38, "layout");

struct UnkTU1123_b257c;
void fn_71007038c0(UnkTU1123_b257c* self);

struct UnkTU1117_l6fbcb0 {
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
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    CVector3D target;  // 0x38
    CVector3D field_0x44;
    u32 field_0x50;
    u8 pad_0x54[0xc8];
    u32 field_0x11c;
};
static_assert(__builtin_offsetof(UnkTU1117_l6fbcb0, field_0x11c) == 0x11c, "layout");

void fn_71006fbcb0(UnkTU1117_l6fbcb0* self) {
    fn_71007038c0(reinterpret_cast<UnkTU1123_b257c*>(self));
    self->vf_0xa0();
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

    UnkSinkParams_l6fbcb0 params;
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
    params.field_0x10.value = self->field_0x11c;
    params.field_0x10.has = true;
    fn_71010c7e6c(reinterpret_cast<UnkEffectSink*>(self->owner->field_0x218), &params, 1);
}
