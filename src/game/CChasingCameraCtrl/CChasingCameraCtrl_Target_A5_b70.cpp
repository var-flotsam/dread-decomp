// CChasingCameraCtrl unit (1202): picks the chased position (+0x68): the followed object's (+0xb8's +0x10, at
// +0x24) or the transform node's translation (+0x18's +0x20, fn_7100206154); the player's x and y replace it per
// the axis flags at +0x1d8. Then it becomes the target (+0x38) with the direction (0, 0, -1) at +0x44. Names and
// layouts are placeholders.
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);

struct UnkChasedPos_A5_b70 {
    f32 x, y, z;
};

struct UnkChasedObject_A5_b70 {
    u8 pad_0x0[0x24];
    UnkChasedPos_A5_b70 pos;
};

struct UnkChasedLink_A5_b70 {
    u8 pad_0x0[0x10];
    UnkChasedObject_A5_b70* object;
};

struct UnkChasingCameraCtrl_A5_b70 {
    u8 pad_0x0[0x18];
    char* node_owner;  // 0x18
    u8 pad_0x20[0x18];
    UnkChasedPos_A5_b70 target;  // 0x38
    f32 dir_x, dir_y, dir_z;     // 0x44
    u8 pad_0x50[0x18];
    UnkChasedPos_A5_b70 chased;  // 0x68
    u8 pad_0x74[0x44];
    UnkChasedLink_A5_b70* link;  // 0xb8
    u8 pad_0xc0[0x118];
    u8 axis_flags;  // 0x1d8
};

void fn_7100797860(UnkChasingCameraCtrl_A5_b70* self) {
    if (self->link && self->link->object) {
        UnkChasedObject_A5_b70* object = self->link->object;
        self->chased.x = object->pos.x;
        self->chased.y = object->pos.y;
        self->chased.z = object->pos.z;
    } else if (self->node_owner) {
        UnkVec3f v = fn_7100206154(reinterpret_cast<UnkXformNode*>(self->node_owner + 0x20));
        self->chased.x = v.x;
        self->chased.y = v.y;
        self->chased.z = v.z;
    }
    if (UnkActor* player = reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310))) {
        f32 y = player->field_0x54;
        if (self->axis_flags & 1)
            self->chased.x = player->field_0x44;
        if (self->axis_flags & 2)
            self->chased.y = y;
    }
    self->target = self->chased;
    self->dir_x = 0;
    self->dir_y = 0;
    self->dir_z = -1.0f;
}
