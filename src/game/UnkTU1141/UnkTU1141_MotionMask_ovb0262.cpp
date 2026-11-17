// Unit 1141: resets a motion (an UnkXform, game/UnkXformNode.h, then a mode word and a vector) for a
// set of motion parts (bits 1, 2 and 4; fn_71007142e8's motions): all parts reset it to the identity
// with the default vector; the angle part only clears the angles, the scale part only the scale.
// Names and layouts are placeholders.
#include "base/math/UnkZeroVector3.h"
#include "game/UnkMotionRule.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU32.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkStruct_71007142e8 {
    UnkXform xform;  // 0x0
    u32 mode;  // 0x5c
    base::math::CVector3D vector;  // 0x60
    f32 field_0x6c;
};
static_assert(sizeof(UnkStruct_71007142e8) == 0x70, "layout");

// The default vector, copied component by component.
__attribute__((always_inline)) static inline void SetVector(UnkStruct_71007142e8* self) {
    self->vector.x = g_unk_710164779c.x;
    self->vector.y = g_unk_710164779c.y;
    self->vector.z = g_unk_710164779c.z;
}

void fn_71007143c8(UnkStruct_71007142e8* self, UnkFlags_71007142e8 flags) {
    if (__builtin_expect(UnkFlagsAll_71007142e8().bits & ~flags.bits, 0)) {
        if (flags.bits & 2) {
            if (self->mode & 4) {
                self->mode = 4;
                SetVector(self);
                self->xform.m[0][3] = 0.0f;
                self->xform.m[1][3] = 0.0f;
                self->xform.m[2][3] = 0.0f;
                return;
            }
        } else if (flags.bits & 4) {
            if (self->mode & 2) {
                self->mode = 2;
                self->xform.SetAngle(UnkVec3f(0.0f, 0.0f, 0.0f));
                return;
            }
        } else {
            return;
        }
    }
    self->field_0x6c = 0.0f;
    SetVector(self);
    fn_7100025a98(&self->xform);
    self->mode = UnkFlagsNone_71007142e8().bits;
}
