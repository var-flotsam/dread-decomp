// tools/tu_map.py unit 53: the square root of fn_71000764d0's squared distance, with all six
// arguments passed through. Names are placeholders.
#include <math.h>

#include "game/UnkTU53.h"
#include "types.h"

f32 fn_71000764a0(const UnkPoint3f* from, const UnkPoint3f* to, const UnkPoint3f* planePoint, UnkPoint3f* outHit,
                  const UnkPoint3f* givenDir, f32* outDist) {
    return sqrtf(fn_71000764d0(from, to, planePoint, outHit, givenDir, outDist));
}
