// Whether the object's target is within the radius at 0x38: false without a position object or
// with a radius that isn't positive, otherwise the squared distance between the two positions
// (values at 0x44, 0x54, 0x64) against the squared radius.
#include "types.h"

struct UnkPositioned {
    u8 pad_0x0[0x44];
    f32 field_0x44;
    u8 pad_0x48[0xc];
    f32 field_0x54;
    u8 pad_0x58[0xc];
    f32 field_0x64;
};

struct UnkRange {
    u8 pad_0x0[0x18];
    UnkPositioned* field_0x18;
    u8 pad_0x20[0x18];
    f32 field_0x38;
};

UnkPositioned* fn_7100d7c990(UnkPositioned* object);

bool fn_710039dd34(UnkRange* self) {
    UnkPositioned* target = fn_7100d7c990(self->field_0x18);
    if (!target)
        return false;
    f32 radius = self->field_0x38;
    if (!(radius > 0.0f))
        return false;
    UnkPositioned* me = self->field_0x18;
    f32 mx = me->field_0x44;
    f32 my = me->field_0x54;
    f32 mz = me->field_0x64;
    f32 tx = target->field_0x44;
    f32 ty = target->field_0x54;
    f32 tz = target->field_0x64;
    f32 dx = mx - tx;
    f32 dy = my - ty;
    return dx * dx + dy * dy + (mz - tz) * (mz - tz) <= radius * radius;
}
