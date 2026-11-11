#include "game/UnkRenderManager.h"
#include "types.h"

bool fn_7100219588(UnkRenderManager*, const void*, u32, u32);

struct UnkPosHolder_b0211 {
    u8 pad_0x0[0x28];
    f32 x;  // 0x28
    f32 y;  // 0x2c
};

struct UnkProbe_b0211 {
    u8 pad_0x0[0x8];
    UnkPosHolder_b0211* target;  // 0x8
};

struct UnkProbePoint_b0211 {
    f32 x, y, z;
};

bool fn_7100a0ea54(UnkProbe_b0211* self, f32 z, const f32* from) {
    f32 x = self->target->x;
    f32 y = self->target->y;
    f32 dx = x - from[0];
    f32 dy = y - from[1];
    f32 len = __builtin_sqrtf(dx * dx + dy * dy);
    if (len != 0.0f) {
        f32 inv = 1.0f / len;
        dx *= inv;
        dy *= inv;
    }
    UnkRenderManager* manager = g_unk_7101d47300;
    UnkProbePoint_b0211 p = {x - dx * 70.7106781f, y - dy * 70.7106781f, z};
    return fn_7100219588(manager, &p, 0, 0);
}
