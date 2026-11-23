// Unit 259 (the graphics system's unit): sorts the frame's lights into sixteen groups by kind and
// shadowing, filling each one's shader data (range, cone, linear colour tinted by the EMMY zone
// colour, ends, shadow parameters), then binds the first shadow map. Names and layouts are placeholders.
#include <math.h>
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "game/UnkTU182.h"
#include "types.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);
void fn_7100160910(u32 index, s32 mode, u32 group);

struct UnkLight_l1cef80 {
    u8 pad_0x0[0x20];
    u32 kind;              // 0x20
    f32 position[3];       // 0x24
    f32 direction[3];      // 0x30
    f32 color[3];          // 0x3c
    u8 pad_0x48[0x7c - 0x48];
    f32 range_near;        // 0x7c
    f32 range_far;         // 0x80
    f32 angle_inner;       // 0x84
    f32 angle_outer;       // 0x88
    u8 pad_0x8c[0xe8 - 0x8c];
    f32 intensity;         // 0xe8
    f32 intensity_scale;   // 0xec
    f32 field_0xf0;
    f32 field_0xf4;
    u8 field_0xf8;
    u8 pad_0xf9[3];
    f32 field_0xfc[4];
    f32 field_0x10c;
    f32 field_0x110;
    f32 field_0x114;
    f32 field_0x118;
    u8 pad_0x11c[4];
    u32 field_0x120;
    u32 field_0x124;
    u8 pad_0x128[0x130 - 0x128];
    void* field_0x130;
    u8 pad_0x138[0x1ba - 0x138];
    u8 field_0x1ba;
    u8 field_0x1bb;
    u8 field_0x1bc;
    u8 field_0x1bd;
    u8 field_0x1be;
};
static_assert(__builtin_offsetof(UnkLight_l1cef80, field_0x1be) == 0x1be, "");

struct UnkLightEntry_l1cef80 {
    u8 pad_0x0[0x78];
    UnkLight_l1cef80* light;  // 0x78
};

struct UnkLightList_l1cef80 {
    UnkLightEntry_l1cef80** data;
    u32 count;

    UnkLightEntry_l1cef80* operator[](u32 i) const {
        if (!(i < count))
            __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
        return data[i];
    }
};

// One light's shader data: 32 floats (eight vec4 rows), indexed by these.
enum : u32 {
    kKind = 0,
    kField4 = 1,
    kField8 = 2,
    kFieldC = 3,
    kRangeNear = 4,
    kRangeFar = 5,
    kConeInner = 6,  // half-angles, in radians
    kConeOuter = 7,
    kColor = 8,
    kField2C = 11,
    kStart = 12,
    kEnd = 16,
    kShadow = 20,
    kField60 = 24,
    kField64 = 25,
    kField68 = 26,
    kField6C = 27,
    kField70 = 28,
};
struct UnkTintEntry_l1cef80 {
    u64 key;
    base::math::CVector4D value;
};

struct UnkShadowMap_l1cef80 {
    u8 pad_0x0[0xa8];
    void* texture;  // 0xa8
};

struct UnkGfx_l1cef80 {
    u8 pad_0x0[0x8];
    UnkTintEntry_l1cef80* tints;  // 0x8
    u64* tint_keys;               // 0x10
    u32 tint_count;               // 0x18
    u8 pad_0x1c[0xd8 - 0x1c];
    UnkShadowMap_l1cef80* maps[4];  // 0xd8
    u32 map_count;                  // 0xf8
    u8 pad_0xfc[0x3183654 - 0xfc];
    f32 field_0x3183654;

    UnkTintEntry_l1cef80* FindTint(u64 key) {
        u32 i = 0;
        for (; i < tint_count; i++) {
            if (tint_keys[i] == key)
                break;
        }
        return &tints[i];
    }
};
static_assert(__builtin_offsetof(UnkGfx_l1cef80, field_0x3183654) == 0x3183654, "");

extern u32 g_unk_716e5548d0[16];
__attribute__((visibility("hidden"))) extern UnkLightEntry_l1cef80* g_unk_716e586940[16][100];
__attribute__((visibility("hidden"))) extern f32 g_unk_716e55493c[16][100][32];

static inline f32 ToRadians(f32 degrees) {
    return degrees * 3.141592653589793 / 180.0;
}

void fn_71001cef80(UnkGfx_l1cef80* self, const UnkLightList_l1cef80* lights) {
    __builtin_memset(g_unk_716e5548d0, 0, sizeof(g_unk_716e5548d0));
    u32 n = lights->count;
    for (u32 i = 0; i < n; i++) {
        UnkLightEntry_l1cef80* entry = (*lights)[i];
        UnkLight_l1cef80* light = entry->light;
        bool shadowed = light->field_0x1ba ? light->field_0x130 != nullptr : false;
        u32 group;
        switch (light->kind) {
        case 1:
            group = shadowed ? 0 : 9;
            break;
        case 2:
            if (light->field_0xf8) {
                if (light->field_0x1bb)
                    group = shadowed ? 2 : 11;
                else
                    group = shadowed ? 4 : 13;
            } else {
                if (light->field_0x1bb)
                    group = shadowed ? 1 : 10;
                else
                    group = shadowed ? 3 : 12;
            }
            break;
        case 3:
            if (!light->field_0x1bb)
                group = shadowed ? 7 : 15;
            else if (light->field_0x1bc)
                group = 8;
            else
                group = shadowed ? 5 : 14;
            break;
        case 5:
            group = 6;
            break;
        default:
            continue;
        }

        u32 slot = g_unk_716e5548d0[group]++;
        g_unk_716e586940[group][slot] = entry;
        f32 intensity = light->intensity * light->intensity_scale;
        intensity = !(intensity < 0.0f) ? intensity : 0.0f;
        u32 kind = light->kind;
        f32* data = g_unk_716e55493c[group][slot];
        data[kKind] = kind;
        data[kField4] = 0.0f;
        data[kField8] = light->field_0xf4 * 0.005f;
        data[kFieldC] = light->field_0xf0 * 20.0f;
        data[kRangeNear] = light->range_near < light->range_far ? light->range_near : light->range_far;
        data[kRangeFar] =
            light->range_near < light->range_far ? light->range_far : light->range_near + 0.001f;
        data[kConeInner] = ToRadians(
            (light->angle_inner < light->angle_outer ? light->angle_inner : light->angle_outer) * 0.5f);
        data[kConeOuter] = ToRadians(
            (light->angle_inner < light->angle_outer ? light->angle_outer : light->angle_inner + 0.001f) *
            0.5f);
        data[kField70 + 0] = light->field_0xfc[0];
        data[kField70 + 1] = light->field_0xfc[1];
        data[kField70 + 2] = light->field_0xfc[2];
        data[kField70 + 3] = light->field_0xfc[3];

        // The colour in linear space, scaled by the intensity.
        f32 r = powf(light->color[0], 2.2f);
        f32 g = powf(light->color[1], 2.2f);
        f32 b = powf(light->color[2], 2.2f);
        base::math::UnkVector4D tint;
        UnkTintEntry_l1cef80* found = self->FindTint(StringCrc64("EMMY_ZONE_LGT", 13));
        r = intensity * r;
        g = intensity * g;
        b = intensity * b;
        if (found) {
            // Blends towards the zone colour at the colour's brightness, by the zone colour's alpha.
            fn_710007b518(&tint, &found->value);
            f32 luminance = (r + g + b) * 0.333f;
            r = r * (1.0f - tint.w) + tint.w * (luminance * tint.x);
            g = g * (1.0f - tint.w) + tint.w * (luminance * tint.y);
            b = b * (1.0f - tint.w) + tint.w * (luminance * tint.z);
        }
        data[kColor + 0] = r;
        data[kColor + 1] = g;
        data[kColor + 2] = b;
        data[kField2C] = light->field_0x1bb ? 1.0f : 0.0f;
        if (kind == 5) {
            data[kStart + 0] = light->position[0] - light->direction[0];
            data[kStart + 1] = light->position[1] - light->direction[1];
            data[kStart + 2] = light->position[2] - light->direction[2];
            data[kStart + 3] = 1.0f;
            data[kEnd + 0] = light->position[0] + light->direction[0];
            data[kEnd + 1] = light->position[1] + light->direction[1];
            data[kEnd + 2] = light->position[2] + light->direction[2];
        } else {
            data[kStart + 0] = light->position[0];
            data[kStart + 1] = light->position[1];
            data[kStart + 2] = light->position[2];
            data[kStart + 3] = 1.0f;
            data[kEnd + 0] = light->direction[0];
            data[kEnd + 1] = light->direction[1];
            data[kEnd + 2] = light->direction[2];
        }
        data[kEnd + 3] = 1.0f;
        if (shadowed) {
            data[kShadow + 0] = light->field_0x114;
            data[kShadow + 1] = 1.0f / light->field_0x120;
            data[kShadow + 2] = 1.0f / light->field_0x124;
            data[kShadow + 3] = light->field_0x118;
        } else {
            data[kShadow + 0] = 0.0f;
            data[kShadow + 1] = 0.0f;
            data[kShadow + 2] = 0.0f;
            data[kShadow + 3] = 0.0f;
        }
        f32 v = light->field_0x110;
        if (!light->field_0x1be)
            v *= self->field_0x3183654;
        data[kField60] = v;
        data[kField64] = light->field_0x1bc ? 1.0f : 0.0f;
        data[kField68] = light->field_0x1bd ? 1.0f : 0.0f;
        data[kField6C] = light->field_0x10c;
    }

    if (self->map_count) {
        fn_71001606f0(0xc, true, 1);
        fn_710015f770(0xc, self->maps[0]->texture, 1);
        fn_7100160810(0xc, 0, 1);
        fn_7100160890(0xc, 0, 1);
        fn_7100160910(0xc, 0, 1);
        fn_7100160990(0xc, 2, 1);
        fn_7100160ab0(0xc, 2, 1);
        fn_7100160d50(0xc, 2, 1);
    } else {
        fn_71001606f0(0xc, false, 1);
    }
}
