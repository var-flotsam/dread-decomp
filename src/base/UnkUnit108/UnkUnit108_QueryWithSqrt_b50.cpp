// From unit 108 of tools/tu_map.py: forwards a query to fn_7100118124 and, when an output distance
// is asked for, turns the squared distance it wrote into a distance. Names are placeholders.
#include <math.h>
#include "types.h"

bool fn_7100118124(void* p0, void* p1, void* p2, void* p3, void* p4, f32* outDistance);

bool fn_7100118274(void* p0, void* p1, void* p2, void* p3, void* p4, f32* outDistance) {
    bool r = fn_7100118124(p0, p1, p2, p3, p4, outDistance);
    if (outDistance)
        *outDistance = sqrtf(*outDistance);
    return r;
}
