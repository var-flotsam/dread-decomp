// Whether the second object is outside the first's radius (at 0x14): true unless the radius is
// positive and the first has a position object whose squared distance to the second's position
// (values at 0x44, 0x54, 0x64) is not below the squared radius.
#include "types.h"

struct UnkPositioned {
    u8 pad_0x0[0x44];
    f32 field_0x44;
    u8 pad_0x48[0xc];
    f32 field_0x54;
    u8 pad_0x58[0xc];
    f32 field_0x64;
};

struct UnkRadius {
    u8 pad_0x0[0x14];
    f32 field_0x14;
};

UnkPositioned* fn_7100d7c990(void* object);

bool fn_71002c36d0(UnkRadius* self, void* first, UnkPositioned* second) {
    if (self->field_0x14 > 0.0f) {
        UnkPositioned* target = fn_7100d7c990(first);
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
            if (radius * radius >= d2)
                return false;
        }
    }
    return true;
}
