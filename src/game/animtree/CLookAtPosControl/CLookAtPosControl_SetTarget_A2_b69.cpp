// animtree::CLookAtPosControl: set the look-at target (position plus the control's offset) and the blend
// time (the default at +0x90 when none is given); when fn_71002d603c refuses, the target becomes current.
#include "base/math/CVectorOps.h"
#include "game/animtree/CLookAtPosControl.h"
#include "types.h"

using base::math::CVector3D;

struct UnkLookAtPosControl_b69 {
    u8 pad_0x0[0x58];
    CVector3D target;   // +0x58
    CVector3D current;  // +0x64
    u8 pad_0x70[0x8];
    CVector3D offset;   // +0x78
    u8 pad_0x84[0xc];
    f32 default_time;  // +0x90
    f32 time;          // +0x94
};

bool fn_71002d603c(UnkLookAtPosControl_b69* self);

void fn_71002e6794(UnkLookAtPosControl_b69* self, const CVector3D& pos, f32 time) {
    self->target = pos + self->offset;
    self->time = !(time <= 0.0f) ? time : self->default_time;
    if (!fn_71002d603c(self))
        self->current = self->target;
}
