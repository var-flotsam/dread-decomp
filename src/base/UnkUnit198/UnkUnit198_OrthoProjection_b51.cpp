// Unit 198: an orthographic camera's projection: the box of its width and height centred on
// the axis, between its near and far planes (fn_7100079a9c), stored in the target's matrix at
// +0x60. Names and layouts are placeholders inferred from the code.

#include "base/UnkMatrix44.h"
#include "types.h"

UnkMatrix44 fn_7100079a9c(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);

struct UnkOrthoParams_7100169494 {
    f32 width;
    f32 height;
    f32 near;
    f32 far;
};

struct UnkProjectionTarget_7100169494 {
    u8 pad_0x0[0x60];
    UnkMatrix44 projection;
};

void fn_7100169494(const UnkOrthoParams_7100169494* self, UnkProjectionTarget_7100169494* target) {
    UnkMatrix44 m = fn_7100079a9c(self->width * -0.5f, self->width * 0.5f, self->height * -0.5f,
                                  self->height * 0.5f, self->near, self->far);
    UnkCopyMatrix(&target->projection, m);
}
