// Scales a 4x4 matrix along y by `scale` (a default scale when it is not positive), then flips
// the sign of element [7].
#include "types.h"

// A 4x4 matrix copied element by element.
struct UnkMatrix44_710015bf40 {
    f32 m[16];
    UnkMatrix44_710015bf40& operator=(const UnkMatrix44_710015bf40& o) {
        for (s32 i = 0; i < 16; ++i)
            m[i] = o.m[i];
        return *this;
    }
};
UnkMatrix44_710015bf40 fn_7100079138(f32 x, f32 y, f32 z);  // scale matrix
UnkMatrix44_710015bf40 fn_71000127c8(const UnkMatrix44_710015bf40& a, const UnkMatrix44_710015bf40& b);  // product
f32 fn_7100132fa0();  // the default scale

void fn_710015bf40(UnkMatrix44_710015bf40* self, f32 scale) {
    if (!(scale > 0.0f))
        scale = fn_7100132fa0();
    *self = fn_71000127c8(fn_7100079138(1.0f, scale, 1.0f), *self);
    self->m[7] = -self->m[7];
}
