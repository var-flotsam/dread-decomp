// tools/tu_map.py unit 53 (0x7100075f78-0x71000769f0): quadratic ease in and out: half of (2t)^2
// below one half, one minus half of (2(1-t))^2 above. Names are placeholders.
#include "types.h"

f32 fn_7100076800(f32 t) {
    if (t < 0.5f) {
        t = t * 2.0f;
        return t * t * 0.5f;
    }
    t = (1.0f - t) * 2.0f;
    return 1.0f - t * t * 0.5f;
}
