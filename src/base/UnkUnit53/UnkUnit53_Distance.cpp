// tools/tu_map.py unit 53: the square root of fn_71000764d0's squared distance, with all six
// arguments passed through. Names are placeholders.
#include <math.h>

#include "types.h"

f32 fn_71000764d0(const void* a, const void* b, void* c, void* d, const void* e, void* f);

f32 fn_71000764a0(const void* a, const void* b, void* c, void* d, const void* e, void* f) {
    return sqrtf(fn_71000764d0(a, b, c, d, e, f));
}
