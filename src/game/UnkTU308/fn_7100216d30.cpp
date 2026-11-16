// Unit 308 (game/UnkSphere.h): scales the sphere's base radius by a factor and caches the radius and its
// fixed-point (x10000) form. Names and layouts are placeholders.
#include "game/UnkSphere.h"
#include "types.h"

void fn_7100216d30(UnkSphere* self, f32 scale) {
    f32 radius = self->base_radius * scale;
    self->field_0x28 = radius;
    self->radius = radius;
    s64 fixed = (s64)(radius * 10000.0f);
    self->radius_fixed = fixed;
    self->field_0x4c = fixed == 0;
}
