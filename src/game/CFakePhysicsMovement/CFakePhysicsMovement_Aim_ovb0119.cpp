// CFakePhysicsMovement's unit (tools/tu_map.py unit 2687): fn_71010cf3f4 aims along the velocity
// (+0x28c) over `t`: the step from the owner's position (+0x30, translation at +0x44/+0x54/+0x64)
// to where the velocity takes it, normalised, is the direction (+0x2c8); the pitch (+0x2d4) and the
// yaw (+0x2d8, -1.57 less the angle in the x/z plane) follow from it, and the three angles from
// +0x2d4 are wrapped (fn_710007a1d0). Returns 0. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVectorOps.h"
#include "game/UnkActor.h"
#include "game/UnkTU54.h"
#include "types.h"

using base::math::CVector3D;

struct UnkFakePhysicsMovement_b0119 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x254];
    CVector3D velocity;  // 0x28c
    u8 pad_0x298[0x30];
    CVector3D direction;  // 0x2c8
    f32 angles[3];                // 0x2d4
};

s32 fn_71010cf3f4(UnkFakePhysicsMovement_b0119* self, f32 t) {
    UnkActor* owner = self->owner;
    CVector3D pos = {owner->field_0x44, owner->field_0x54, owner->field_0x64};
    CVector3D target = pos + self->velocity * t;
    CVector3D& d = self->direction;
    d = target - pos;
    CVector3D n = Normalized(d);
    d = n;
    self->angles[0] = -atan2f(-n.y, sqrtf(n.x * n.x + n.z * n.z));
    self->angles[1] = -1.57f - atan2f(d.z, d.x);
    f32 a = fn_710007a1d0(self->angles[0]);
    f32 b = fn_710007a1d0(self->angles[1]);
    f32 c = fn_710007a1d0(self->angles[2]);
    self->angles[0] = a;
    self->angles[1] = b;
    self->angles[2] = c;
    return 0;
}
