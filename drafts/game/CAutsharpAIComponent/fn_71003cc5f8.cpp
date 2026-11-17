// CAutsharpAIComponent unit (tools/tu_map.py unit 572): fn_71003cc5f8 adjusts the movement output after the
// base routine: while turning, it stops the x motion when moving against the facing, and reports the turn's
// end once facing ahead and at the target value. Names and layouts are placeholders.
#include "base/math/CVectorOps.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkAIScaleSource_b49;
struct UnkAIScaleTarget_b49;
void fn_7101111c24(UnkAIScaleSource_b49* self, UnkAIScaleTarget_b49* out);

struct UnkAutsharpTurnEvent_l3cc5f8 {
    f32 value;
    u32 field_0x4;
    u32 field_0x8;
};

struct UnkAutsharpTurner_l3cc5f8 {
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
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void Notify(const UnkAutsharpTurnEvent_l3cc5f8* event, bool flag);  // 0xd0

    u8 pad_0x8[0x3c];
    f32 value;  // 0x44
    u8 pad_0x48[0xc];
    u32 field_0x54;
    u8 pad_0x58[0xc];
    u32 field_0x64;
};

struct UnkAutsharpOutput_l3cc5f8 {
    u8 pad_0x0[0x60];
    CVector3D a;  // 0x60
    CVector3D b;  // 0x6c
};

struct UnkAutsharpAI_l3cc5f8 {
    u8 pad_0x0[0x30];
    UnkAutsharpTurner_l3cc5f8* turner;  // 0x30
    u8 pad_0x38[0x146d1];
    bool field_0x14709;
    bool field_0x1470a;
    u8 pad_0x1470b;
    bool field_0x1470c;
    u8 pad_0x1470d[0x5f];
    CVector2D facing;  // 0x1476c
    f32 target;  // 0x14774
    u8 pad_0x14778[8];
    bool turning;  // 0x14780
    u8 pad_0x14781[4];
    bool stopped;  // 0x14785
};
static_assert(__builtin_offsetof(UnkAutsharpAI_l3cc5f8, field_0x14709) == 0x14709, "layout");
static_assert(__builtin_offsetof(UnkAutsharpAI_l3cc5f8, facing) == 0x1476c, "layout");
static_assert(__builtin_offsetof(UnkAutsharpAI_l3cc5f8, stopped) == 0x14785, "layout");

// Whether a is within eps of b.
inline bool IsNear(f32 a, f32 b, f32 eps) {
    return a - eps <= b && a + eps >= b;
}

void fn_71003cc5f8(UnkAutsharpAI_l3cc5f8* self, UnkAutsharpOutput_l3cc5f8* out) {
    fn_7101111c24(reinterpret_cast<UnkAIScaleSource_b49*>(self), reinterpret_cast<UnkAIScaleTarget_b49*>(out));
    if (self->turning) {
        CVector2D dir;
        fn_710007b430(&dir, &out->a);
        Normalize(dir);
        f32 dot = Dot(self->facing, dir);
        if (dot < 0.0f || IsNear(dot, 0.0f, 0.01f)) {
            out->a.x = 0.0f;
            out->b.x = 0.0f;
            if (IsNear(dot, 0.0f, 0.01f)) {
                UnkAutsharpTurner_l3cc5f8* turner = self->turner;
                f32 target = self->target;
                f32 value = turner->value;
                if (IsNear(value, target, 0.1f)) {
                    UnkAutsharpTurnEvent_l3cc5f8 event;
                    event.value = target;
                    event.field_0x4 = turner->field_0x54;
                    event.field_0x8 = turner->field_0x64;
                    turner->Notify(&event, true);
                }
            }
        }
    } else if (self->stopped) {
        out->a.x = 0.0f;
        out->b.x = 0.0f;
    } else if (self->field_0x1470a) {
        out->a.x = out->a.x * 0.5f;
        out->b.x = out->b.x * 0.5f;
    } else if (!self->field_0x14709 && !self->field_0x1470c) {
        out->a.y = 0.0f;
        out->b.y = 0.0f;
    }
}
