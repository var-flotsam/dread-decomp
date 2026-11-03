#include "types.h"

struct UnkVec3 {
    f32 x, y, z;
};

// The offset after time `t` of a shot launched along (dirY, dirX) at `speed` under `gravity`
// (scaled by `t` once more): x = dirX * speed * t, y = dirY * speed * t - gravity * t * t / 2.
void fn_71009fa8fc(UnkVec3* out, f32 dirY, f32 dirX, f32 speed, f32 gravity, f32 t) {
    f32 vx = dirX * speed;
    f32 vy = dirY * speed;
    out->z = 0.0f;
    f32 g = gravity * t;
    out->x = vx * t;
    out->y = vy * t + g * t * -0.5f;
}
