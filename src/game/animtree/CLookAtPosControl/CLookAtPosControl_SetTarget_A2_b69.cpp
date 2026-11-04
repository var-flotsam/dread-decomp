// animtree::CLookAtPosControl: set the look-at target (position plus the control's offset) and the blend
// time (the default at +0x90 when none is given); when fn_71002d603c refuses, the target becomes current.
#include "types.h"

struct Vec3_b69 {
    f32 x, y, z;
    Vec3_b69 operator+(const Vec3_b69& o) const {
        Vec3_b69 r = *this;
        r.x += o.x;
        r.y += o.y;
        r.z += o.z;
        return r;
    }
};

struct UnkLookAtPosControl_b69 {
    u8 pad_0x0[0x58];
    Vec3_b69 target;   // +0x58
    Vec3_b69 current;  // +0x64
    u8 pad_0x70[0x8];
    Vec3_b69 offset;   // +0x78
    u8 pad_0x84[0xc];
    f32 default_time;  // +0x90
    f32 time;          // +0x94
};

bool fn_71002d603c(UnkLookAtPosControl_b69* self);

void fn_71002e6794(UnkLookAtPosControl_b69* self, const Vec3_b69& pos, f32 time) {
    self->target = pos + self->offset;
    self->time = !(time <= 0.0f) ? time : self->default_time;
    if (!fn_71002d603c(self))
        self->current = self->target;
}
