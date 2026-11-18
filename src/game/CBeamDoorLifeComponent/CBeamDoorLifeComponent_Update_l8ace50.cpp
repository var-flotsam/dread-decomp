// CBeamDoorLifeComponent unit (tools/tu_map.py): advances the timer of the current state; a state up to 2
// ends once its timer reaches its duration. Names and layouts are placeholders.
#include "types.h"

struct UnkBeamDoorLife_l8ace50 {
    u8 pad_0x0[0x494];
    s32 state;     // 0x494
    f32 time;      // 0x498
    f32 duration;  // 0x49c
};

void fn_71008ace50(UnkBeamDoorLife_l8ace50* self, f32 dt) {
    s32 state = self->state;
    if (state != 0)
        self->time += dt;
    else
        self->time = 0.0f;
    bool expired = (state <= 2) & (self->time >= self->duration);
    if (expired) {
        self->state = 0;
        self->time = 0.0f;
    }
}
