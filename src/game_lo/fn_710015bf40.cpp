// Scales a 4x4 matrix along y by `scale` (a default scale when it is not positive), then flips
// the sign of element [7].
#include "base/UnkMatrix44.h"
#include "types.h"

f32 fn_7100132fa0();  // the default scale

void fn_710015bf40(UnkMatrix44* self, f32 scale) {
    if (!(scale > 0.0f))
        scale = fn_7100132fa0();
    UnkCopyMatrix(self, fn_71000127c8(fn_7100079138(1.0f, scale, 1.0f), *self));
    self->m[7] = -self->m[7];
}
