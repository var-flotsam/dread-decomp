#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkEmmyMovement_A3_b66;
void fn_7100571660(UnkEmmyMovement_A3_b66* self);
void fn_7100969130(char* self);
void fn_710007b444(void* dst, const void* value);

struct UnkInner_b334 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkTarget_b334 {
    void** vtable;
    u8 pad_0x8[0x120];
    UnkInner_b334* field_0x128;
    u8 pad_0x130[0xc];
    base::math::CVector3D field_0x13c;
    u8 pad_0x148[0xc];
    u8 field_0x154[0xc];

    UnkVec3f vf_0x370() {
        return reinterpret_cast<UnkVec3f (*)(UnkTarget_b334*)>(vtable[0x370 / 8])(this);
    }
};

struct UnkAI_b334 {
    u8 pad_0x0[0x70];
    u32 count;  // 0x70
    u8 pad_0x74[0x20];
    u8 field_0x94;
};

struct UnkOwner_b334 {
    u8 pad_0x0[0x240];
    UnkAI_b334* ai;  // 0x240
};

struct UnkUnit1442Start {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkOwner_b334* owner;  // 0x30
    UnkTarget_b334* target;  // 0x38
    u8 pad_0x40[0x49];
    u8 field_0x89;
    u8 pad_0x8a[2];
    f32 field_0x8c;
    f32 field_0x90;
    u8 pad_0x94[0x14];
    u8 field_0xa8;
    u8 pad_0xa9[0x4b];
    f32 field_0xf4;
    f32 field_0xf8;
    f32 field_0xfc;
    f32 field_0x100;
    u8 pad_0x104[0x39];
    u8 field_0x13d;

    void vf_0xa0() { reinterpret_cast<void (*)(UnkUnit1442Start*)>(vtable[0xa0 / 8])(this); }
};

void fn_710096b334(UnkUnit1442Start* self) {
    fn_7100969130(reinterpret_cast<char*>(self));
    UnkTarget_b334* target = self->target;
    self->field_0xa8 = 0;
    self->field_0x13d = 1;
    if (target->field_0x128 && target->field_0x128->field_0x10) {
        base::math::CVector2D out;
        fn_710007b430(&out, &target->field_0x13c);
        self->field_0x8c = out.x;
        const void* axis = target->field_0x154;
        self->field_0x90 = out.y;
        fn_710007b444(&self->field_0xfc, axis);
        target = self->target;
    }
    if (target) {
        if (fn_710009d50c(reinterpret_cast<base::reflection::UnkReflObject*>(target)->vf_0x0(), fn_710057134c()))
            fn_7100571660(reinterpret_cast<UnkEmmyMovement_A3_b66*>(target));
    }
    UnkVec3f v = self->target->vf_0x370();
    f32 y = -self->field_0xfc;
    self->field_0xf4 = self->field_0x100;
    self->field_0xf8 = y;
    base::math::CVector2D p;
    fn_710007b430(&p, reinterpret_cast<const base::math::CVector3D*>(&v));
    f32 px = p.x, py = p.y;
    f32 dx = self->field_0xf4;
    f32 dy = self->field_0xf8;
    f32 along_y = dy * py;
    f32 along_x = dx * px;
    f32 dot = along_x + along_y;
    if (dot < -0.001f || (dot < 0.001f && dot > -0.001f && !self->field_0x89)) {
        dx = -dx;
        dy = -dy;
        self->field_0xf4 = dx;
        self->field_0xf8 = dy;
    }
    u32 bx, by;
    __builtin_memcpy(&bx, &dx, 4);
    __builtin_memcpy(&by, &dy, 4);
    __builtin_memcpy(&self->field_0xf4, &bx, 4);
    __builtin_memcpy(&self->field_0xf8, &by, 4);
    if (UnkAI_b334* ai = self->owner->ai) {
        u32 count = ai->count;
        bool open = ai->field_0x94 == 0;
        if (count != ((count != 0) & open))
            self->vf_0xa0();
    }
}
