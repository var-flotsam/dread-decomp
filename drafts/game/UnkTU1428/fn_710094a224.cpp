// CMorphBallMovement's unit (hint only): asks the movement to move to a target; on success runs its follow-up and
// writes back the node translation the request changed, else remembers the target. Names and layouts are placeholders.

#include "game/CCharacterMovement.h"
#include "game/UnkXformNode.h"

struct UnkBallNode_l94a224 {
    u8 pad_0x0[0x44];
    f32 x_0x44;
    u8 pad_0x48[0xc];
    f32 y_0x54;
    u8 pad_0x58[0xc];
    f32 z_0x64;
    u8 pad_0x68[0x348];
    u8 dirty_0x3b0;
};

struct UnkBallTarget_l94a224 {
    u8 pad_0x0[0x158];
    void* field_0x158;
};

struct UnkBallMovement_l94a224;

struct UnkBallMovementVtbl_l94a224 {
    u8 pad_0x0[0x200];
    bool (*vf_0x200)(UnkBallMovement_l94a224* self, UnkVec3f* position, void* unused, u32 flags, void* shape, s32 mode);
    u8 pad_0x208[0xa8];
    void (*vf_0x2b0)(UnkBallMovement_l94a224* self, UnkBallTarget_l94a224* target);
};

struct UnkBallMovement_l94a224 {
    UnkBallMovementVtbl_l94a224* vtable;
    u8 pad_0x8[0x28];
    UnkBallNode_l94a224* node;
    u8 pad_0x38[0x390];
    u32 field_0x3c8;
    u8 pad_0x3cc[0x2234];
    UnkBallTarget_l94a224* field_0x2600;
};

void fn_710094a224(UnkBallMovement_l94a224* self, UnkBallTarget_l94a224* target) {
    if (reinterpret_cast<UnkBallTarget_l94a224*>(fn_7100976588(reinterpret_cast<UnkFlagged*>(self))) == target)
        return;
    UnkBallNode_l94a224* node = self->node;
    UnkVec3f current(node->x_0x44, node->y_0x54, node->z_0x64);
    UnkVec3f position = current;
    UnkBallTarget_l94a224* remembered = target;
    if (self->vtable->vf_0x200(self, &position, nullptr, self->field_0x3c8, target->field_0x158, 3)) {
        self->vtable->vf_0x2b0(self, target);
        if (UNK_CHANGED(position.x, current.x) || UNK_CHANGED(position.y, current.y) ||
            UNK_CHANGED(position.z, current.z)) {
            node = self->node;
            node->y_0x54 = position.y;
            node->x_0x44 = position.x;
            node->z_0x64 = position.z;
            node->dirty_0x3b0 = 1;
        }
        remembered = nullptr;
    }
    self->field_0x2600 = remembered;
}
