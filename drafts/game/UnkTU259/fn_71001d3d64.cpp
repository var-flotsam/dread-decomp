// Unit 259 (the graphics system's unit): sets one light's shader constants (kind, range, cone,
// position, direction and colour) on the overlay program, by the light's kind.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector4D.h"
#include "game/UnkTU180.h"
#include "game/UnkTU230.h"
#include "game/UnkTU54.h"
#include "types.h"
#include <math.h>

struct UnkLight_l1d3d64 {
    u8 pad_0x0[0x20];
    u32 kind;                         // 0x20
    UnkVec3f position;                // 0x24
    UnkVec3f direction;               // 0x30
    base::math::CVector4D color;      // 0x3c
    u8 pad_0x4c[0x7c - 0x4c];
    f32 range_near;                   // 0x7c
    f32 range_far;                    // 0x80
    f32 cone_inner;                   // 0x84
    f32 cone_outer;                   // 0x88
    u8 pad_0x8c[0xe8 - 0x8c];
    f32 intensity;                    // 0xe8
    f32 intensity_scale;              // 0xec
    u8 pad_0xf0[0x130 - 0xf0];
    void* field_0x130;
    u8 pad_0x138[0x1ba - 0x138];
    u8 field_0x1ba;
};
static_assert(__builtin_offsetof(UnkLight_l1d3d64, field_0x1ba) == 0x1ba, "");

struct UnkLightEntry_l1d3d64 {
    UnkLight_l1d3d64* light;
    f32 field_0x8;
};

struct UnkGfx_l1d3d64 {
    u8 pad_0x0[0x317af68];
    void* state;    // 0x317af68
    void* program;  // 0x317af70
    u8 pad_0x317af78[0x318366f - 0x317af78];
    u8 transform_lights;  // 0x318366f
};
static_assert(__builtin_offsetof(UnkGfx_l1d3d64, transform_lights) == 0x318366f, "");

static const f32 kDegToRad = 0.017453292f;

// The light's range, as near and far (far kept above near), with the far end pushed out by one.
inline void SetRange(UnkGfx_l1d3d64* self, u32 base, const UnkLight_l1d3d64* light, u64* location) {
    if (fn_71001aa0b4(self->program, 1, base + 0x10, location)) {
        f32 a = light->range_near;
        f32 b = light->range_far;
        f32 rangeNear = a < b ? a : b;
        f32 rangeFar = a < b ? b : a + 0.001f;
        f32 values[4] = {rangeNear, rangeFar + 1.0f, 0.0f, 0.0f};
        fn_710015e1f0(self->state, 1, *location, 4, values);
    }
}

// The light's position, in view space when the lights are transformed.
inline void SetPosition(UnkGfx_l1d3d64* self, u32 base, const UnkLight_l1d3d64* light,
                        const UnkMatrix44* matrix, u64* location) {
    if (fn_71001aa0b4(self->program, 1, base + 0x15, location)) {
        UnkVec3f position = light->position;
        if (self->transform_lights)
            position = fn_7100078170(reinterpret_cast<const UnkMat4f*>(matrix), &position);
        f32 values[4] = {position.x, position.y, position.z, 0.0f};
        fn_710015e1f0(self->state, 1, *location, 4, values);
    }
}

void fn_71001d3d64(UnkGfx_l1d3d64* self, u32 index, const UnkLightEntry_l1d3d64* entry,
                   const UnkMatrix44* matrix) {
    u64 location;
    if (!self->program)
        return;
    UnkLight_l1d3d64* light = entry->light;
    f32 param = entry->field_0x8;
    u32 base = index * 9;
    u32 kind = light->kind;
    bool shadowed = light->field_0x1ba && light->field_0x130 != nullptr;
    if (fn_71001aa0b4(self->program, 1, base + 0xf, &location)) {
        f32 values[4] = {(f32)kind, shadowed ? 1.0f : 0.0f, param, 1.0f};
        fn_710015e1f0(self->state, 1, location, 4, values);
    }

    switch (kind) {
    case 2:
        if (fn_71001aa0b4(self->program, 1, base + 0x10, &location)) {
            f32 a = light->range_near;
            f32 b = light->range_far;
            f32 rangeNear = a < b ? a : b;
            f32 rangeFar = a < b ? b : a + 0.001f;
            f32 c = light->cone_inner;
            f32 d = light->cone_outer;
            f32 coneInner = c < d ? c : d;
            f32 coneOuter = c < d ? d : c + 0.001f;
            f32 cosInner = cosf(coneInner * 0.5f * kDegToRad);
            f32 cosOuter = cosf(coneOuter * 0.5f * kDegToRad);
            f32 values[4] = {rangeNear, rangeFar, cosOuter, cosInner};
            fn_710015e1f0(self->state, 1, location, 4, values);
        }
        if (fn_71001aa0b4(self->program, 1, base + 0x15, &location)) {
            UnkVec3f position = light->position;
            if (self->transform_lights)
                position = fn_7100078170(reinterpret_cast<const UnkMat4f*>(matrix), &position);
            f32 values[4] = {position.x, position.y, position.z, 0.0f};
            fn_710015e1f0(self->state, 1, location, 4, values);
        }
        // fall through: a spot light has a direction too
    case 1:
        if (fn_71001aa0b4(self->program, 1, base + 0x16, &location)) {
            UnkVec3f direction = light->direction;
            if (self->transform_lights)
                direction = fn_7100077c04(*matrix, direction);
            f32 values[4] = {direction.x, direction.y, direction.z, 0.0f};
            fn_710015e1f0(self->state, 1, location, 4, values);
        }
        break;
    case 3:
        SetRange(self, base, light, &location);
        SetPosition(self, base, light, matrix, &location);
        break;
    case 5:
        SetRange(self, base, light, &location);
        SetPosition(self, base, light, matrix, &location);
        if (fn_71001aa0b4(self->program, 1, base + 0x16, &location)) {
            UnkVec3f direction = light->direction;
            if (self->transform_lights)
                direction = fn_7100078170(reinterpret_cast<const UnkMat4f*>(matrix), &direction);
            f32 values[4] = {direction.x, direction.y, direction.z, 0.0f};
            fn_710015e1f0(self->state, 1, location, 4, values);
        }
        break;
    }

    f32 intensity = light->intensity;
    f32 scale = light->intensity_scale;
    if (fn_71001aa0b4(self->program, 1, base + 0x11, &location)) {
        base::math::CVector4D color = fn_710007b5c0(&light->color, intensity * scale);
        fn_710015e1f0(self->state, 1, location, 4, &color.x);
    }
}
