// CFloorShockWaveComponent's unit: the wave's direction along x, +1 or -1 by the sign of the
// y angle of the linked object's transform (+0x1c8 -> +0x10, transform at +0x38), +1 without one.
// Names are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkFloorShockWaveTarget_A5_b66 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkFloorShockWaveLink_A5_b66 {
    u8 pad_0x0[0x10];
    UnkFloorShockWaveTarget_A5_b66* target;  // 0x10
};

struct UnkFloorShockWave_A5_b66 {
    u8 pad_0x0[0x1c8];
    UnkFloorShockWaveLink_A5_b66* link;  // 0x1c8
};

UnkVec3f fn_71007e6ce0(UnkFloorShockWave_A5_b66* self) {
    UnkVec3f dir(0.0f, 0.0f, 0.0f);
    bool positive = true;
    if (self->link) {
        if (UnkFloorShockWaveTarget_A5_b66* target = self->link->target)
            positive = target->xform.GetAngle().y > 0.0f;
    }
    f32 x = positive ? 1.0f : -1.0f;
    dir = UnkVec3f(x, 0.0f, 0.0f);
    return dir;
}
