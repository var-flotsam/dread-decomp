// CAreaFXComponent unit (tools/tu_map.py unit 1263): where a target on a slanted surface meets the rectangle
// record: 0 for a surface along an axis or one facing away from the rectangle's centre, otherwise the target's
// point moved along the surface by its angle, and 2 for a diagonal surface, 1 for any other. Names and
// layouts are placeholders.
#include "game/CAreaFXComponent.h"
#include "types.h"

extern "C" f32 fmodf(f32, f32);

typedef u32 __attribute__((may_alias)) a_u32_ovb0081;

struct UnkAreaFXResult_A5_b66;

struct UnkAreaFXPoint_ovb0081 {
    f32 x, y;
    UnkAreaFXPoint_ovb0081() : x(0.0f), y(0.0f) {}
    UnkAreaFXPoint_ovb0081(const UnkAreaFXPoint_ovb0081& o) : x(o.x), y(o.y) {}
    void Set(const UnkAreaFXResult_A5_b66& r) {
        ((a_u32_ovb0081*)this)[0] = ((const a_u32_ovb0081*)&r)[0];
        ((a_u32_ovb0081*)this)[1] = ((const a_u32_ovb0081*)&r)[1];
    }
};

struct UnkAreaFXTarget_l7fdd80 {
    u8 pad_0x0[0x14];
    f32 x;  // 0x14
    f32 y;  // 0x18
    u8 pad_0x1c[0x10];
    f32 dir_x;  // 0x2c
    f32 dir_y;  // 0x30
};

// Whether v is within a thousandth of target.
static inline bool IsNear_l7fdd80(f32 v, f32 target) {
    return v - 0.001f <= target && v + 0.001f >= target;
}

s32 fn_71007fdd80(UnkAreaFXRecord_A5_b66 record, void* target, UnkAreaFXPoint_ovb0081* out) {
    const UnkAreaFXTarget_l7fdd80* t = static_cast<const UnkAreaFXTarget_l7fdd80*>(target);
    f32 ax = __builtin_fabsf(t->dir_x);
    f32 ay = __builtin_fabsf(t->dir_y);
    if (IsNear_l7fdd80(ax + ay, 1.0f))
        return 0;
    f32 center = (record.field_0x0 + record.field_0x8) * 0.5f;
    f32 offset = t->x - center;
    if (t->dir_x * offset >= 0.0f)
        return 0;
    s32 result = IsNear_l7fdd80(ax, ay) ? 2 : 1;
    out->x = t->x;
    out->y = t->y;
    f32 angle = fmodf(t->y, 90.0f);
    if (!IsNear_l7fdd80(angle, 0.0f)) {
        f32 dir_x = t->dir_x;
        f32 dir_y = t->dir_y;
        f32 shift;
        if (dir_y > 0.0f)
            shift = -__builtin_fabsf(angle < 0.0f ? angle + 90.0f : angle);
        else
            shift = __builtin_fabsf(angle > 0.0f ? angle - 90.0f : angle);
        out->x -= dir_y * (shift / dir_x);
        out->y = shift + out->y;
    }
    return result;
}
