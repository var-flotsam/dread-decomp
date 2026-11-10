// CFakePhysicsMovement's unit (tools/tu_map.py unit 2687): fn_71010cf3f4 aims along the velocity
// (+0x28c) over `t`: the step from the owner's position (+0x30, translation at +0x44/+0x54/+0x64)
// to where the velocity takes it, normalised, is the direction (+0x2c8); the pitch (+0x2d4) and the
// yaw (+0x2d8, -1.57 less the angle in the x/z plane) follow from it, and the three angles from
// +0x2d4 are wrapped (fn_710007a1d0). Returns 0. Names and layouts are placeholders.
#include <math.h>

#include "game/UnkActor.h"
#include "types.h"

f32 fn_710007a1d0(f32 angle);  // wraps an angle into (-pi, pi]

struct UnkFakeVec3_b0119 {
    f32 x, y, z;
};

struct UnkFakePhysicsMovement_b0119 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x254];
    UnkFakeVec3_b0119 velocity;  // 0x28c
    u8 pad_0x298[0x30];
    UnkFakeVec3_b0119 direction;  // 0x2c8
    f32 angles[3];                // 0x2d4
};

s32 fn_71010cf3f4(UnkFakePhysicsMovement_b0119* self, f32 t) {
    UnkActor* owner = self->owner;
    f32 px = owner->field_0x44;
    f32 py = owner->field_0x54;
    f32 pz = owner->field_0x64;
    f32 tx = px + self->velocity.x * t;
    f32 ty = py + self->velocity.y * t;
    f32 tz = pz + self->velocity.z * t;
    f32 dx = tx - px;
    f32 dy = ty - py;
    f32 dz = tz - pz;
    UnkFakeVec3_b0119& d = self->direction;
    d.x = dx;
    d.y = dy;
    d.z = dz;
    f32 inv = 1.0f / (sqrtf(d.x * d.x + d.y * d.y + d.z * d.z) + 0.0001f);
    f32 nx = d.x * inv;
    f32 ny = d.y * inv;
    f32 nz = d.z * inv;
    d.x = nx;
    d.y = ny;
    d.z = nz;
    self->angles[0] = -atan2f(-ny, sqrtf(nx * nx + nz * nz));
    self->angles[1] = -1.57f - atan2f(d.z, d.x);
    f32 a = fn_710007a1d0(self->angles[0]);
    f32 b = fn_710007a1d0(self->angles[1]);
    f32 c = fn_710007a1d0(self->angles[2]);
    self->angles[0] = a;
    self->angles[1] = b;
    self->angles[2] = c;
    return 0;
}
