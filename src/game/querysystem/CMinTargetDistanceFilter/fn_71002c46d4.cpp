// Whether the second object is outside the first's radius (at 0x14): true unless the radius is
// positive and the first has a position object whose squared distance to the second's position
// (values at 0x44, 0x54, 0x64) is not below the squared radius.
#include "game/UnkActor.h"
#include "types.h"

struct UnkRadius {
    u8 pad_0x0[0x14];
    f32 field_0x14;
};

bool fn_71002c46d4(UnkRadius* self, UnkActor* first, UnkActor* second) {
    if (self->field_0x14 > 0.0f) {
        UnkActor* target = fn_7100d7c990(first);
        if (target) {
            f32 tx = target->field_0x44;
            f32 ty = target->field_0x54;
            f32 tz = target->field_0x64;
            f32 sx = second->field_0x44;
            f32 sy = second->field_0x54;
            f32 sz = second->field_0x64;
            f32 dx = tx - sx;
            f32 dy = ty - sy;
            f32 dz = tz - sz;
            f32 d2 = dx * dx + dy * dy + dz * dz;
            f32 radius = self->field_0x14;
            if (radius * radius <= d2)
                return false;
        }
    }
    return true;
}
