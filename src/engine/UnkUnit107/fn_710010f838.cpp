// From unit 107 of tools/tu_map.py (0x710010d604-0x7100110c54, no reflected class): whether a
// capsule's centre lies inside a circle, within `tolerance`. The shapes are laid out in
// src/engine/SystemStartup.cpp (a circle: centre x, y and radius; a capsule: centre x, y first);
// they are read as floats here so that no other file's layout is repeated.
#include "types.h"

struct UnkCircle2D;
struct UnkCapsule2D;

bool fn_710010f838(const UnkCircle2D* circle, const UnkCapsule2D* capsule, f32 tolerance) {
    const f32* c = reinterpret_cast<const f32*>(circle);
    const f32* p = reinterpret_cast<const f32*>(capsule);
    f32 dx = c[0] - p[0];
    f32 dy = c[1] - p[1];
    f32 distance = dx * dx + dy * dy;
    return c[2] * c[2] - distance >= -(tolerance * tolerance);
}
