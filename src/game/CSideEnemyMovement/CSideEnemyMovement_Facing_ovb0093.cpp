// CSideEnemyMovement unit (tools/tu_map.py): fn_71009b5f48 turns the movement's up vector
// (fn_71010c9ad0, the CEnemyMovement one) into an angle about z, keeps the owner's transform angle y
// (+0x30, +0x38, derived lazily), builds the vector for those angles (fn_710007a220), hands it to the
// virtual at slot 0x380 and returns it. Names and layouts are placeholders.
#include <math.h>

#include "game/CEnemyMovement.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkEnemyMovement_A5;

struct UnkSideEnemyOwner_ovb0093 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkSideEnemyMovement2_ovb0093 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkSideEnemyOwner_ovb0093* owner;  // 0x30

    void vf_0x380(const UnkVec3f& v, f32 t) {
        reinterpret_cast<void (*)(UnkSideEnemyMovement2_ovb0093*, const UnkVec3f&, f32)>(vtable[0x380 / 8])(this, v, t);
    }
};

UnkVec3f fn_71009b5f48(UnkSideEnemyMovement2_ovb0093* self) {
    UnkVec3f up = fn_71010c9ad0(reinterpret_cast<UnkEnemyMovement_A5*>(self));
    f32 angle = -(atan2f(-up.y, -up.x) + 1.5707964f);
    UnkVec3f angles(angle, self->owner->xform.GetAngle().y, 0.0f);
    UnkVec3f result = fn_710007a220(&angles);
    self->vf_0x380(result, 0.0f);
    return result;
}
