// CThermalDeviceComponent unit (tools/tu_map.py, name from reflection): fn_7100a93208 runs the
// update step (fn_7100a9fad8), then while the timer at +0x598 runs, counts it down by `dt` and sets
// the w of the render parameter named by the hash g_unk_710164b5f8 to 1 - 2 * timer (read with
// fn_710021a0bc, written with fn_710021a0b4); 1 once the timer runs out. Names and layouts are
// placeholders.
#include "base/math/CVector4D.h"
#include "game/CUsableComponent.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU315.h"
#include "types.h"

using base::math::CVector4D;

struct UnkThermalDevice_b50a5 {
    u8 unk_0x0[0x598];
    f32 timer;  // 0x598
};

extern const u64 g_unk_710164b5f8 __attribute__((visibility("hidden")));

void fn_7100a93208(UnkThermalDevice_b50a5* self, f32 dt) {
    fn_7100a9fad8(self, dt);
    if (self->timer > 0.0f) {
        CVector4D v = {0.0f, 0.0f, 0.0f, 0.0f};
        fn_710021a0bc(g_unk_7101d47300, &g_unk_710164b5f8, &v);
        f32 t = self->timer;
        v.w = 1.0f - (t + t);
        self->timer = t - dt;
        if (self->timer < 0.0f) {
            self->timer = 0.0f;
            v.w = 1.0f;
        }
        fn_710021a0b4(g_unk_7101d47300, &g_unk_710164b5f8, &v);
    }
}
