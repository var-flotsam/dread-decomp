// CSpotLightComponent unit: sets one colour of the light's render data from a vector, clamping each
// component to be non-negative. Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "base/math/CVectorOps.h"
#include "types.h"

using base::math::CVector4D;

struct UnkLightDataB {
    u8 pad_0x0[0x3c];
    CVector4D color;
};

struct UnkLightRenderB {
    u8 pad_0x0[0x78];
    UnkLightDataB* data;
};

struct UnkLightOwnerB {
    u8 pad_0x0[0x138];
    UnkLightRenderB* render;
};

struct UnkSpotLightB {
    u8 pad_0x0[0x58];
    UnkLightOwnerB* owner;
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

void fn_71008f94e4(UnkSpotLightB* self, const CVector4D* value) {
    CVector4D* dst = &self->owner->render->data->color;
    CVector4D c = UnkClampColor(value);
    Assign(*dst, c);
}
