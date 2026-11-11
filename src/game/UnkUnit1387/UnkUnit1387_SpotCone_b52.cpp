// Unit 1387 of tools/tu_map.py (no reflected class): applies a spot light's settings to its light
// (+0x120, its data at +0x138 -> +0x78), fn_71008e9f70. The intensity (+0xac, at least 0) and the
// range (+0xb0, at least 5; fn_710020c9ac) are set, then the inner and outer cone angles (+0xb4,
// +0xb8, clamped to 0..180 degrees, the outer one raised to 5 first) with the cosines of their halves; the light is marked dirty
// (+0x1b8). Names and layouts are placeholders.
#include "game/UnkTU290.h"
#include "types.h"

extern "C" f32 cosf(f32);

struct UnkSpotLightData_b52 {
    u8 unk_0x0[0x7c];
    f32 intensity;   // 0x7c
    u8 unk_0x80[4];
    f32 inner;       // 0x84
    f32 outer;       // 0x88
    f32 inner_cos;   // 0x8c
    f32 outer_cos;   // 0x90
    u8 unk_0x94[0x124];
    bool dirty;      // 0x1b8
};

struct UnkSpotLightRender_b52 {
    u8 unk_0x0[0x78];
    UnkSpotLightData_b52* data;
};

struct UnkSpotLight_b52 {
    u8 unk_0x0[0x20];
    u8 field_0x20[0x118];
    UnkSpotLightRender_b52* render;  // 0x138
};

struct UnkSpotLightComponent_b52 {
    u8 unk_0x0[0xac];
    f32 intensity;  // 0xac
    f32 range;      // 0xb0
    f32 inner;      // 0xb4
    f32 outer;      // 0xb8
    u8 unk_0xbc[0x64];
    UnkSpotLight_b52* light;  // 0x120
};

// Stored through an access that may alias anything: the original reads the light again after it.
typedef f32 __attribute__((may_alias)) UnkAliasF32_b52;

static inline f32 Max_b52(f32 a, f32 b) {
    return a < b ? b : a;
}

static inline f32 Angle_b52(f32 v, f32 least) {
    v = Max_b52(v, least);
    return !(v <= 0.0f) ? (!(v >= 180.0f) ? v : 180.0f) : 0.0f;
}

static inline f32 HalfCos_b52(f32 degrees) {
    return cosf((f32)((f64)(degrees * 0.5f) * 3.141592653589793 / 180.0));
}

void fn_71008e9f70(UnkSpotLightComponent_b52* self) {
    f32 intensity = self->intensity;
    f32 range = self->range;
    f32 inner = self->inner;
    f32 outer = self->outer;
    *reinterpret_cast<UnkAliasF32_b52*>(&self->light->render->data->intensity) = Max_b52(intensity, 0.0f);
    fn_710020c9ac(self->light->field_0x20, Max_b52(range, 5.0f));
    UnkSpotLightData_b52* data = self->light->render->data;
    f32 a = Angle_b52(inner, 0.0f);
    data->inner = a;
    data->inner_cos = HalfCos_b52(a);
    data = self->light->render->data;
    a = Angle_b52(outer, 5.0f);
    data->outer = a;
    data->outer_cos = HalfCos_b52(a);
    data->dirty = true;
}
