// Sphere overlap test: centre distance (via sqrtf) below the sum of the radii.
#include "types.h"

extern "C" f32 sqrtf(f32);

struct UnkSphere {
    u8 unk_0x0[0x8];
    f32 radius;
    u8 unk_0xc[0xc];
    f32 x, y, z;
};

bool fn_7100216c2c(const UnkSphere* a, const UnkSphere* b) {
    f32 dx = a->x - b->x;
    f32 dy = a->y - b->y;
    f32 dz = a->z - b->z;
    return sqrtf(dx * dx + dy * dy + dz * dz) < a->radius + b->radius;
}
