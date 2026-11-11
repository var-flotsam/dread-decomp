// CAreaFXComponent unit (tools/tu_map.py unit 1263): fn_7100802108, the angle step of the update
// (CAreaFXComponent_b41.cpp, CAreaFXComponent_b52.cpp): when an angle is set (+0x414) and differs from
// the owner's (its transform's cached angles, within 0.001 per axis), flags the owner's object at
// +0x240 (+0x16e6) and turns the owner (vtable slot 0xd8). Names and layouts are placeholders.
#include "game/CAreaFXComponent.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkSelf_af41;

struct UnkFlagHolder_ovb0081 {
    u8 pad_0x0[0x16e6];
    u8 field_0x16e6;
};

struct UnkOwner_ovb0081 {
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
    virtual void vf_0xd0(const f32* pos, bool flag);
    virtual void vf_0xd8(const f32* angle, bool flag);

    u8 pad_0x8[0x30];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x1ac];
    UnkFlagHolder_ovb0081* field_0x240;
};
static_assert(__builtin_offsetof(UnkOwner_ovb0081, field_0x240) == 0x240, "UnkOwner_ovb0081 layout");

struct UnkAreaFX_ovb0081 {
    u8 pad_0x0[0x30];
    UnkOwner_ovb0081* owner;
    u8 pad_0x38[0x3dc];
    u8 has_angle;  // 0x414
    u8 pad_0x415[3];
    f32 angle[3];  // 0x418
};

static inline bool Near_ovb0081(f32 a, f32 b) {
    f32 lo = a - 0.001f;
    f32 hi = a + 0.001f;
    return lo <= b && hi >= b;
}

void fn_7100802108(UnkSelf_af41* p) {
    UnkAreaFX_ovb0081* self = reinterpret_cast<UnkAreaFX_ovb0081*>(p);
    if (!self->has_angle)
        return;
    UnkVec3f a = self->owner->xform.GetAngle();
    if (Near_ovb0081(a.x, self->angle[0]) && Near_ovb0081(a.y, self->angle[1]) &&
        Near_ovb0081(a.z, self->angle[2]))
        return;
    if (UnkFlagHolder_ovb0081* holder = self->owner->field_0x240)
        *((u8*)holder + 0x16e6) = 1;
    self->owner->vf_0xd8(self->angle, true);
}
