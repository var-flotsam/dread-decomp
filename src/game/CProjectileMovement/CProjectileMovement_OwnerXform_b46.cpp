// CProjectileMovement unit (tools/tu_map.py unit 1623): copies the owner's transform angles
// (derived lazily, include/game/UnkXformNode.h) and translation into the movement. The layouts
// are placeholders.
#include "game/CProjectileMovement.h"
#include "game/UnkXformNode.h"

struct UnkProjectileOwner_b46a3 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkProjectileMovement_b46a3 {
    u8 pad_0x0[0x30];
    UnkProjectileOwner_b46a3* owner;  // 0x30
    u8 pad_0x38[0x4c];
    UnkVec3f position;  // 0x84
    UnkVec3f angle;     // 0x90
};

void fn_7100ac40fc(UnkProjectileMovement_b46a3* self, f32) {
    self->angle = self->owner->xform.GetAngle();
    self->position = self->owner->xform.GetTranslation();
}
