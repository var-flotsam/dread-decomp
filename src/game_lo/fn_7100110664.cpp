// Sets an angle (wrapped into (-pi, pi]) and rebuilds the 2x2 rotation for the angle plus a
// stored offset.
#include <math.h>

#include "types.h"

f32 fn_710007a1d0(f32 angle);  // wraps an angle into (-pi, pi]

struct UnkStruct_7100110664 {
    u8 unk_0x0[0x8];
    f32 rot[4];  // cos, sin, -sin, cos
    u8 unk_0x18[0x8];
    f32 offset;
    f32 angle;
};

void fn_7100110664(UnkStruct_7100110664* self, f32 angle) {
    self->angle = fn_710007a1d0(angle);
    self->rot[0] = cosf(self->angle + self->offset);
    self->rot[1] = sinf(self->angle + self->offset);
    self->rot[2] = -sinf(self->angle + self->offset);
    self->rot[3] = cosf(self->angle + self->offset);
}
