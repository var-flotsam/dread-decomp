// COmniLightComponent: clamps each component of a colour to be non-negative and stores it in the
// light's data. Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "types.h"

using base::math::CVector4D;

struct UnkLightData_l8f1a68 {
    u8 pad_0x0[0x4c];
    CVector4D color;
};

struct UnkLightOwner_l8f1a68 {
    u8 pad_0x0[0x78];
    UnkLightData_l8f1a68* data;
};

struct UnkLightBase_l8f1a68 {
    u8 pad_0x0[0x140];
    UnkLightOwner_l8f1a68* owner;
};

struct UnkOmniLight_l8f1a68 {
    u8 pad_0x0[0x58];
    UnkLightBase_l8f1a68* base;
};

static inline f32 UnkClampPositive(f32 x) { return x <= 0.0f ? 0.0f : x; }

static inline CVector4D UnkClampColor(const CVector4D* v) {
    CVector4D c;
    c.x = UnkClampPositive(v->x);
    c.y = UnkClampPositive(v->y);
    c.z = UnkClampPositive(v->z);
    c.w = UnkClampPositive(v->w);
    return c;
}

void fn_71008f1a68(UnkOmniLight_l8f1a68* self, const CVector4D* value) {
    CVector4D* color = &self->base->owner->data->color;
    CVector4D c = UnkClampColor(value);
    fn_710007b518(color, &c);
}
