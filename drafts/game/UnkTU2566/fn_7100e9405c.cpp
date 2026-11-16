// UnkTU2566 (the minimap's unit, tools/tu_map.py unit 2566): moves the minimap's focus towards a target
// point: straight to it while the bounds are empty, else eased between two clamps of where the camera
// shows it, then snapped to a grid of 10. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkTU2566.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

void fn_7100e94a6c(UnkMinimapOwner* self);

struct UnkMinimapCamera_le9405c {
    u8 pad_0x0[0x20];
    UnkXformNode node;  // 0x20
};

struct UnkMinimapBounds_le9405c {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
};

struct UnkMinimapFollow_le9405c {
    u8 pad_0x0[0xbc];
    f32 field_0xbc;
    u8 pad_0xc0[0x4];
    f32 field_0xc4;
    u8 pad_0xc8[0xd63d40 - 0xc8];
    UnkMinimapBounds_le9405c bounds;  // 0xd63d40: where the focus may move
    CVector2D pos;                    // 0xd63d50: the focus
    u8 pad_0xd63d58[0x1f0];
    UnkMinimapCamera_le9405c* camera;  // 0xd63f48
};
static_assert(__builtin_offsetof(UnkMinimapFollow_le9405c, camera) == 0xd63f48, "layout");

static inline f32 Clamp(f32 v, f32 lo, f32 hi) {
    return v > lo ? (v >= hi ? hi : v) : lo;
}

void fn_7100e9405c(UnkMinimapOwner* owner, const void* v) {
    UnkMinimapFollow_le9405c* self = reinterpret_cast<UnkMinimapFollow_le9405c*>(owner);
    const CVector2D* target = static_cast<const CVector2D*>(v);
    const UnkMinimapBounds_le9405c& bounds = self->bounds;
    CVector2D& pos = self->pos;
    fn_7100e94a6c(owner);
    if (bounds.max_x >= bounds.min_x && bounds.max_y >= bounds.min_y) {
        UnkVec3f world = fn_7100206154(&self->camera->node);
        CVector2D shown;
        fn_710007b430(&shown, reinterpret_cast<const CVector3D*>(&world));
        f32 dx = shown.x - pos.x;
        f32 dy = shown.y - pos.y;
        f32 x = dx + target->x;
        f32 y = dy + target->y;
        f32 outer_x = Clamp(x, bounds.min_x + -900.0f, bounds.max_x + 900.0f);
        f32 outer_y = Clamp(y, bounds.min_y + -900.0f, bounds.max_y + 900.0f);
        f32 inner_x = Clamp(x, bounds.min_x + -900.0f + 450.0f, bounds.max_x + 900.0f + -450.0f);
        f32 inner_y = Clamp(y, bounds.min_y + -900.0f + 450.0f, bounds.max_y + 900.0f + -450.0f);
        f32 t = self->field_0xbc / self->field_0xc4 * 0.3f;
        pos.x = t * (inner_x - dx) + (outer_x - dx) * (1.0f - t);
        pos.y = (outer_y - dy) * (1.0f - t) + t * (inner_y - dy);
    } else {
        pos.x = target->x;
        pos.y = target->y;
    }
    pos.x = __builtin_roundf(pos.x * 0.1f) * 10.0f;
    pos.y = __builtin_roundf(pos.y * 0.1f) * 10.0f;
}
