// CCameraFX_Shake's unit (tools/tu_map.py): the shake's strength at a position, scaled by `scale`.
// While enabled (+0x30) with a falloff range (inner +0x38, outer +0x3c), the distance from the
// centre (+0x24) maps to 0..1 across the range, then to 1..factor (+0x40), and the curve named by
// the string id at +0x48 shapes it (fn_7100076020, as in
// src/game/querysystem/CEvaluator/CEvaluator_ShapedScore_A2_b68.cpp). Names and layouts are
// placeholders.
#include "base/global/CFilePathStrId.h"
#include "game/UnkTU53.h"
#include "types.h"

struct UnkShakeStrData_ovb0108 {
    u8 pad_0x0[0x10];
    const char* text;
    u8 pad_0x18[0x44 - 0x18];
    union {
        u32 use_count;
    };
};

struct UnkShakePos_ovb0108 {
    f32 x, y;
};

struct UnkCameraShake_ovb0108 {
    u8 pad_0x0[0x24];
    UnkShakePos_ovb0108 centre;  // 0x24
    u8 pad_0x2c[0x4];
    bool enabled;  // 0x30
    u8 pad_0x31[0x7];
    f32 inner;   // 0x38
    f32 outer;   // 0x3c
    f32 factor;  // 0x40
    u8 pad_0x44[0x4];
    union {
        UnkShakeStrData_ovb0108* curve;  // 0x48
    };
};

static inline f32 Saturate(f32 t) {
    return !(t <= 0.0f) ? (!(t >= 1.0f) ? t : 1.0f) : 0.0f;
}

f32 fn_71010f3f20(UnkCameraShake_ovb0108* self, const UnkShakePos_ovb0108* pos, f32 scale) {
    f32 value = 1.0f;
    if (self->enabled && self->outer > 0.0f && self->inner < self->outer) {
        f32 dx = pos->x - self->centre.x;
        f32 dy = pos->y - self->centre.y;
        f32 distance = __builtin_sqrtf(dx * dx + dy * dy + 0.0f);
        f32 v = (self->factor - 1.0f) * Saturate((distance - self->inner) / (self->outer - self->inner)) + 1.0f;
        ++self->curve->use_count;
        const char* text = self->curve->text;
        value = fn_7100076020(v, text ? text : g_unk_710162e2ed);
    }
    return value * scale;
}
