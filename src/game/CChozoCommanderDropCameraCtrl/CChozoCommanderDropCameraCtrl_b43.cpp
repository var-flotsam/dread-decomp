// CChozoCommanderDropCameraCtrl unit (tools/tu_map.py, reflection hint). fn_7100798868: until the
// controller's progress (+0x2c) reaches 0.85, the camera-boundary value (fn_7100765484, in the
// CCameraBoundaryCtrl unit) plus the offset at +0x2ac; zero after. Names and layouts are placeholders.
#include "types.h"

struct UnkDropCameraCtrl_b43a2 {
    u8 pad_0x0[0x2c];
    f32 progress;  // 0x2c
    u8 pad_0x30[0x27c];
    f32 offset;  // 0x2ac
};

f32 fn_7100765484(UnkDropCameraCtrl_b43a2* self);

f32 fn_7100798868(UnkDropCameraCtrl_b43a2* self) {
    if (self->progress >= 0.85f)
        return 0.0f;
    return fn_7100765484(self) + self->offset;
}
