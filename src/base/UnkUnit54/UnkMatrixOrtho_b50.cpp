// From unit 54 of tools/tu_map.py: an orthographic projection matrix (row-major, translation in the
// fourth column) for the box left..right, bottom..top, near..far, mapping depth to [0, 1].
// Names are placeholders.
#include "base/UnkMatrix44.h"
#include "types.h"

UnkMatrix44 fn_7100079a9c(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far) {
    UnkMatrix44 r;
    __builtin_memset(&r, 0, sizeof(r));
    r.m[0] = r.m[5] = r.m[10] = r.m[15] = 1.0f;  // starts as the identity
    r.m[0] = 2.0f / (right - left);
    r.m[5] = 2.0f / (top - bottom);
    r.m[10] = 1.0f / (near - far);
    r.m[3] = (left + right) / (left - right);
    r.m[7] = (bottom + top) / (bottom - top);
    r.m[11] = near / (near - far);
    return r;
}
