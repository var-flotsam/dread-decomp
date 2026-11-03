// Unit 1382 (tools/tu_map.py, no name hint): the lights of
// src/game/CCharClassLightingComponent/CCharClassLightingComponent_2.cpp and their fades. A fade with
// a duration (not within 0.001 of zero) copies the current value to the start, the given value to
// the target and sets the duration and a cleared elapsed time; otherwise all three take the value at
// once and a virtual slot of the light applies it. fn_71008e5f14 and fn_71008e5f8c fade two 3D
// values (at +0x1c and +0x48, applied through slots 0x78 and 0x80), fn_71008e6004 the colour
// (+0x74, slot 0x88). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "base/math/UnkZeroVector3.h"
#include "types.h"

using base::math::CVector3D;
using base::math::CVector4D;
using base::math::UnkVector4D;

// Declared as src/game/CCharClassLightingComponent/CCharClassLightingComponent_2.cpp does.
struct UnkLightingLight2;

struct UnkFade3D_b42 {
    CVector3D current;
    CVector3D start;
    CVector3D target;
    f32 duration;
    f32 elapsed;
};

struct UnkLight_b42 {
    void** vtable;
    u8 pad_0x8[0x14];
    UnkFade3D_b42 fade_a;  // 0x1c
    UnkFade3D_b42 fade_b;  // 0x48
    CVector4D current;  // 0x74
    CVector4D start;  // 0x84
    CVector4D target;  // 0x94
    f32 duration;  // 0xa4
    f32 elapsed;  // 0xa8

    void vf_0x88() { reinterpret_cast<void (*)(UnkLight_b42*)>(vtable[0x88 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkLight_b42, current) == 0x74, "UnkLight_b42 layout");

static inline __attribute__((always_inline)) void Set3D_b42(CVector3D& v, f32 x, f32 y, f32 z) {
    v.x = x;
    v.y = y;
    v.z = z;
}

#define UNK_FADE3D_B42(addr, member, slot)                                                 \
    void fn_##addr(UnkLightingLight2* light, void* arg, f32 value) {                       \
        UnkLight_b42* self = reinterpret_cast<UnkLight_b42*>(light);                       \
        const CVector3D* color = static_cast<const CVector3D*>(arg);                       \
        UnkFade3D_b42& fade = self->member;                                                \
        if (value + 0.001f >= 0.0f && value - 0.001f <= 0.0f) {                           \
            f32 cx = color->x, cy = color->y, cz = color->z;                              \
            Set3D_b42(fade.target, cx, cy, cz);                                            \
            Set3D_b42(fade.start, cx, cy, cz);                                             \
            Set3D_b42(fade.current, cx, cy, cz);                                           \
            reinterpret_cast<void (*)(UnkLight_b42*)>(self->vtable[slot / 8])(self);       \
        } else {                                                                           \
            f32 cx = color->x, cy = color->y, cz = color->z;                              \
            *reinterpret_cast<u64*>(&self->member.start.x) =                               \
                *reinterpret_cast<const u64*>(&self->member.current.x);                    \
            self->member.start.z = self->member.current.z;                                 \
            Set3D_b42(fade.target, cx, cy, cz);                                            \
            fade.duration = value;                                                         \
            fade.elapsed = 0.0f;                                                           \
        }                                                                                  \
    }

UNK_FADE3D_B42(71008e5f14, fade_a, 0x78)
UNK_FADE3D_B42(71008e5f8c, fade_b, 0x80)

// The assignment returns its destination here (the original passes it on to the next one); the
// header declares it without a result, so it is called through this cast.
static inline CVector4D* Assign_b42(CVector4D* self, const CVector4D* other) {
    return reinterpret_cast<CVector4D* (*)(CVector4D*, const CVector4D*)>(&fn_710007b518)(self, other);
}

// Both paths take the colour by value (a copy each).
static inline void SetAt_b42(UnkLight_b42* self, CVector4D* current, UnkVector4D color) {
    CVector4D* target = Assign_b42(&self->target, &color);
    CVector4D* start = Assign_b42(&self->start, target);
    Assign_b42(current, start);
    self->vf_0x88();
}

static inline void FadeTo_b42(UnkLight_b42* self, CVector4D* current, UnkVector4D color,
                              f32 duration) {
    Assign_b42(&self->start, current);
    Assign_b42(&self->target, &color);
    self->duration = duration;
    self->elapsed = 0.0f;
}

void fn_71008e6004(UnkLightingLight2* light, void* new_color, f32 duration) {
    UnkLight_b42* self = reinterpret_cast<UnkLight_b42*>(light);
    const UnkVector4D* color = static_cast<const UnkVector4D*>(new_color);
    if (duration + 0.001f >= 0.0f && duration - 0.001f <= 0.0f)
        SetAt_b42(self, &self->current, *color);
    else
        FadeTo_b42(self, &self->current, *color, duration);
}

// fn_71008e61f4: the same fade over one value (+0x10c, applied through slot 0xa8). Declared as
// src/game/CCharClassLightingComponent/CCharClassLightingComponent_b36.cpp does.
struct UnkLightingLight3;

struct UnkFade1D_b42 {
    f32 current;
    f32 start;
    f32 target;
    f32 duration;
    f32 elapsed;
};

struct UnkLightScalar_b42 {
    void** vtable;
    u8 pad_0x8[0x104];
    UnkFade1D_b42 fade;  // 0x10c
};

void fn_71008e61f4(UnkLightingLight3* light, f32 a, f32 b) {
    UnkLightScalar_b42* self = reinterpret_cast<UnkLightScalar_b42*>(light);
    if (b + 0.001f >= 0.0f && b - 0.001f <= 0.0f) {
        self->fade.target = a;
        self->fade.start = a;
        self->fade.current = a;
        reinterpret_cast<void (*)(UnkLightScalar_b42*)>(self->vtable[0xa8 / 8])(self);
    } else {
        self->fade.start = self->fade.current;
        self->fade.target = a;
        self->fade.duration = b;
        self->fade.elapsed = 0.0f;
    }
}

// fn_71008e5e24: re-applies everything: slot 0x38 with the definition's flag (+0x8, its +0x5e),
// slots 0x48 and 0x58 with what slots 0x50 and 0x60 report, then slots 0x78 to 0xa8.
struct UnkLightDesc_b42 {
    u8 pad_0x0[0x5e];
    bool field_0x5e;
};

struct UnkLightApply_b42 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38(bool);
    virtual void vf_0x40();
    virtual void vf_0x48(bool);
    virtual bool vf_0x50();
    virtual void vf_0x58(bool);
    virtual bool vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    UnkLightDesc_b42* desc;  // 0x8
};

void fn_71008e5e24(UnkLightApply_b42* self) {
    self->vf_0x38(self->desc->field_0x5e);
    self->vf_0x48(self->vf_0x50());
    self->vf_0x58(self->vf_0x60());
    self->vf_0x78();
    self->vf_0x80();
    self->vf_0x88();
    self->vf_0x90();
    self->vf_0x98();
    self->vf_0xa0();
    self->vf_0xa8();
}

// fn_71008e62b8: a light's 3D value for a mode: modes 1 and 4 give the zero vector, 3 and 5 the
// value with z cleared, the others the value as is.
// g_unk_710164779c: base/math/UnkZeroVector3.h

// Returned in memory: the vector type has a user-written copy constructor.
struct UnkVec3_b42 {
    f32 x, y, z;
    UnkVec3_b42(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
    UnkVec3_b42(const UnkVec3_b42& o) : x(o.x), y(o.y), z(o.z) {}
};

UnkVec3_b42 fn_71008e62b8(const UnkVec3_b42* value, s32 mode) {
    switch (mode) {
    case 1:
    case 4:
        return reinterpret_cast<const UnkVec3_b42&>(g_unk_710164779c);
    case 3:
    case 5:
        return UnkVec3_b42(value->x, value->y, 0.0f);
    default:
        return *value;
    }
}
