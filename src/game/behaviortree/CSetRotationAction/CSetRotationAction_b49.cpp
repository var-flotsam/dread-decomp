// behaviortree::CSetRotationAction (tools/tu_map.py): the node's update. Takes the owner's (+0x18)
// angles (its transform at +0x38, game/UnkXformNode.h), replaces each component whose flag is set
// (+0x2c..+0x2e) by the node's value (+0x30..+0x38), stores them back, marks the owner's +0x3c0 and
// returns 5 (done). Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkSetRotationOwner_b49 {
    u8 pad_0x0[0x38];
    UnkXform transform;
    u8 pad_0x94[0x32c];
    bool field_0x3c0;
};

struct UnkSetRotationAction_b49 {
    u8 pad_0x0[0x18];
    UnkSetRotationOwner_b49* owner;
    u8 pad_0x20[0xc];
    bool set_x;
    bool set_y;
    bool set_z;
    u8 pad_0x2f;
    f32 x;
    f32 y;
    f32 z;
};

u32 fn_7100391268(UnkSetRotationAction_b49* self) {
    UnkVec3f angle = self->owner->transform.GetAngle();
    if (self->set_x)
        angle.x = self->x;
    if (self->set_y)
        angle.y = self->y;
    if (self->set_z)
        angle.z = self->z;
    UnkSetRotationOwner_b49* owner = self->owner;
    owner->transform.SetAngle(angle);
    owner->field_0x3c0 = true;
    return 5;
}
