// CRedenkiSwarmControllerComponent unit (tools/tu_map.py, the name from reflection): 1000 in mode 2
// with the state at +0x530 in 2..4; otherwise the angle over pi, clamped to [0, 1], picks between
// 200 and 400. Names are placeholders.
#include "types.h"

struct UnkRedenkiSpeed_b64a4 {
    u8 pad_0x0[0x390];
    s32 mode;  // 0x390
    u8 pad_0x394[0x19c];
    u32 state;  // 0x530
};

f32 fn_7100658944(UnkRedenkiSpeed_b64a4* self, f32 angle) {
    if (self->mode == 2 && self->state - 2 < 3)
        return 1000.0f;
    f32 t = angle / 3.14159274f;
    if (t >= 1.0f)
        return 400.0f;
    if (t < 0.0f)
        return 200.0f;
    return (1.0f - t) * 200.0f + t * 400.0f;
}
