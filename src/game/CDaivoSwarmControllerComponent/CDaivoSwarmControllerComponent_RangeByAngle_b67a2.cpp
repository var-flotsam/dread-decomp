// CDaivoSwarmControllerComponent unit (tools/tu_map.py, the name from reflection): a distance for an angle:
// 1000 in mode 2 (+0x390); otherwise the angle over pi, clamped to [0, 1], blends 300 into 600. Names and
// layouts are placeholders.
#include "types.h"

struct UnkDaivoSwarmController_b67a2 {
    u8 pad_0x0[0x390];
    u32 mode;
};

f32 fn_71004e288c(UnkDaivoSwarmController_b67a2* self, f32 angle) {
    if (self->mode == 2)
        return 1000.0f;
    f32 t = angle / 3.14159274f;
    if (t >= 1.0f)
        return 600.0f;
    if (t < 0.0f)
        return 300.0f;
    f32 near = (1.0f - t) * 300.0f;
    return t * 600.0f + near;
}
