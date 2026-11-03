// Unit 1382 (tools/tu_map.py, no name hint): fn_71008e60fc, a copy of fn_71008e6004
// (src/game/UnkUnit1382/UnkUnit1382_b42.cpp) for the lights of
// src/game/CCharClassLightingComponent/CCharClassLightingComponent.cpp: a colour fade. With a duration
// (not within 0.001 of zero) the current colour is copied to the start, the given one to the target and
// the duration set with a cleared elapsed time; otherwise all three take the colour at once and a
// virtual slot of the light applies it. Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "types.h"

using base::math::CVector4D;
using base::math::UnkVector4D;

// Declared as src/game/CCharClassLightingComponent/CCharClassLightingComponent.cpp does.
struct UnkLightingLight;

struct UnkLight_b43a2 {
    void** vtable;
    u8 pad_0x8[0xa4];
    CVector4D current;  // 0xac
    CVector4D start;  // 0xbc
    CVector4D target;  // 0xcc
    f32 duration;  // 0xdc
    f32 elapsed;  // 0xe0

    void vf_0x90() { reinterpret_cast<void (*)(UnkLight_b43a2*)>(vtable[0x90 / 8])(this); }
};

// The assignment returns its destination here (the original passes it on to the next one); the
// header declares it without a result, so it is called through this cast.
static inline CVector4D* Assign_b43a2(CVector4D* self, const CVector4D* other) {
    return reinterpret_cast<CVector4D* (*)(CVector4D*, const CVector4D*)>(&fn_710007b518)(self, other);
}

// Both paths take the colour by value (a copy each).
static inline void SetAt_b43a2(UnkLight_b43a2* self, CVector4D* current, UnkVector4D color) {
    CVector4D* target = Assign_b43a2(&self->target, &color);
    CVector4D* start = Assign_b43a2(&self->start, target);
    Assign_b43a2(current, start);
    self->vf_0x90();
}

static inline void FadeTo_b43a2(UnkLight_b43a2* self, CVector4D* current, UnkVector4D color,
                                f32 duration) {
    Assign_b43a2(&self->start, current);
    Assign_b43a2(&self->target, &color);
    self->duration = duration;
    self->elapsed = 0.0f;
}

void fn_71008e60fc(UnkLightingLight* light, void* arg, f32 value) {
    UnkLight_b43a2* self = reinterpret_cast<UnkLight_b43a2*>(light);
    const UnkVector4D* color = static_cast<const UnkVector4D*>(arg);
    if (value + 0.001f >= 0.0f && value - 0.001f <= 0.0f)
        SetAt_b43a2(self, &self->current, *color);
    else
        FadeTo_b43a2(self, &self->current, *color, value);
}
