// CMissileMovement unit: after the movement's update (fn_7100ac3880), the velocity at +0x4b8 is the
// direction at +0x40 scaled by the speed of the tunable data at +0x5f0 (its +0x134). Names and
// layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CProjectileMovement.h"
#include "types.h"

using base::math::CVector3D;

struct UnkMissileTunables {
    u8 pad_0x0[0x134];
    f32 speed;  // 0x134
};

struct UnkMissileMovement {
    u8 pad_0x0[0x40];
    CVector3D direction;  // 0x40
    u8 pad_0x4c[0x46c];
    CVector3D velocity;  // 0x4b8
    u8 pad_0x4c4[0x12c];
    UnkMissileTunables* tunables;  // 0x5f0
};
static_assert(__builtin_offsetof(UnkMissileMovement, tunables) == 0x5f0, "UnkMissileMovement layout");

struct UnkSelf;

void fn_7100aba644(UnkMissileMovement* self) {
    fn_7100ac3880(reinterpret_cast<UnkSelf*>(self));
    f32 speed = self->tunables->speed;
    const CVector3D& d = self->direction;
    CVector3D v = {speed * d.x, speed * d.y, speed * d.z};
    self->velocity = v;
}
