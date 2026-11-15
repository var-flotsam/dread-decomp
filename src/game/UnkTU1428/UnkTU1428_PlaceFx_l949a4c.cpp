// CMorphBallMovement's unit (hint only): places the movement's two effect objects at a position, turned to face a
// direction, and switches them on or off. Names and layouts are placeholders.

#include <math.h>

#include "game/UnkSwitchObject.h"
#include "game/UnkTU134.h"
#include "game/UnkXformNode.h"

struct UnkBallFx_l949a4c {
    u8 pad_0x0[0x254c];
    u8 field_0x254c;
    u8 pad_0x254d[0x63];
    UnkFxObject_ovb0041* field_0x25b0;
    UnkFxObject_ovb0041* field_0x25b8;
};

static inline void PlaceFx(UnkFxObject_ovb0041** fx, const UnkVec3f* position, const f32* direction, bool flip) {
    f32 yaw = atan2f(direction[1], direction[0]);
    f32 roll = flip ? 3.14159274f : 0.0f;
    fn_7100146d84(*fx, position);
    UnkFxObject_ovb0041* target = *fx;
    UnkVec3f angle;
    angle.x = roll;
    angle.y = 0.0f;
    angle.z = yaw;
    fn_7100146d98(target, &angle);
}

void fn_7100949a4c(UnkBallFx_l949a4c* self, const UnkVec3f* position, const f32* direction, bool flip) {
    if (self->field_0x25b0) {
        PlaceFx(&self->field_0x25b0, position, direction, flip);
        if (self->field_0x254c)
            fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(self->field_0x25b0), false);
        else
            fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(self->field_0x25b0), true);
    }
    if (self->field_0x25b8) {
        PlaceFx(&self->field_0x25b8, position, direction, flip);
        if (self->field_0x254c)
            fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(self->field_0x25b8), true);
        else
            fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(self->field_0x25b8), false);
    }
}
