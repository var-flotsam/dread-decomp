// CInterpolationComponent unit: copies a transform's translation (mask bit 1) and its angles
// (bit 2, derived on demand) into +0x40 and +0x4c. Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkInterpolationXformState {
    u8 pad_0x0[0x40];
    UnkVec3f position;  // 0x40
    UnkVec3f angle;     // 0x4c
};

void fn_710116e6d0(UnkInterpolationXformState* self, UnkXform* xform, const u32* mask) {
    if (*mask & 1)
        self->position = xform->GetTranslation();
    if (*mask & 2)
        self->angle = xform->GetAngle();
}
