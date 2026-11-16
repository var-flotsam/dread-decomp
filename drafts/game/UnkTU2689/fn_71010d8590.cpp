// Unit 2689 (CPlayerMovement?): the movement speed for the current input, picked by mode,
// signed by the input direction and scaled by the input's factors.
// Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkTU2689SpeedNode {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};
static_assert(__builtin_offsetof(UnkTU2689SpeedNode, xform) == 0x38, "xform");

struct UnkTU2689SpeedOwner {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkTU2689SpeedInput {
    u8 pad_0x0[0x8];
    UnkTU2689SpeedOwner* owner_0x8;
    u8 pad_0x10[0x39];
    u8 has_dir_0x49;
    f32 dir_x_0x4c;
    f32 dir_y_0x50;
    f32 scale_x_0x54;
    f32 scale_y_0x58;
    u8 pad_0x5c[0x6c];
    s32 mode_0xc8;
};
static_assert(__builtin_offsetof(UnkTU2689SpeedInput, mode_0xc8) == 0xc8, "mode");

struct UnkTU2689SpeedFacing {
    f32 x;
    f32 y;
};

struct UnkTU2689Speed {
    u8 pad_0x0[0x30];
    UnkTU2689SpeedNode* node_0x30;
    u8 pad_0x38[0xc1];
    u8 alt_0xf9;
    u8 pad_0xfa[0x6d2];
    f32 speed_x_0x7cc;
    f32 speed_y_0x7d0;
    f32 speed_z_0x7d4;
    f32 fast_x_0x7d8;
    f32 fast_y_0x7dc;
    f32 fast_z_0x7e0;
};
static_assert(__builtin_offsetof(UnkTU2689Speed, speed_x_0x7cc) == 0x7cc, "speed");

UnkVec3f fn_71010d8590(UnkTU2689Speed* self, const UnkTU2689SpeedFacing* facing,
                       const UnkTU2689SpeedInput* input) {
    f32 x = self->speed_x_0x7cc;
    f32 z = self->speed_z_0x7d4;
    f32 y = self->speed_y_0x7d0;
    switch (input->mode_0xc8) {
    case 1:
        x += (self->fast_x_0x7d8 - x) * 0.5f;
        z += (self->fast_z_0x7e0 - z) * 0.5f;
        y += (self->fast_y_0x7dc - y) * 0.5f;
        break;
    case 2:
        x = self->fast_x_0x7d8;
        break;
    case 3:
    case 4:
        x = self->fast_x_0x7d8;
        z = self->fast_z_0x7e0;
        y = self->fast_y_0x7dc;
        break;
    case 5:
        x = self->fast_x_0x7d8 * 1.5f;
        z = self->fast_z_0x7e0 * 1.5f;
        y = self->fast_y_0x7dc * 1.5f;
        break;
    }

    UnkVec3f result(0.0f, 0.0f, 0.0f);
    if (input->has_dir_0x49) {
        result.x = !(input->dir_x_0x4c >= 0.0f) ? -x : x;
    } else if (input->owner_0x8 != nullptr && input->owner_0x8->field_0x10 != nullptr) {
        result.x = facing->y > 0.0f ? -x : x;
    } else {
        result.x = self->node_0x30->xform.GetAngle().y > 0.0f ? -x : x;
    }

    result.y = self->alt_0xf9 == 0 ? y : z;
    if (input->has_dir_0x49 && !(input->dir_y_0x50 >= 0.0f)) {
        result.y = -result.y;
    }

    result.x *= input->scale_x_0x54;
    result.y *= input->scale_y_0x58;
    return result;
}
