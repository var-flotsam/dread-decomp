// CVulkranMagmaBallMovementComponent's unit: the hit test. When the ball hits the player and the player
// is in the state at fn_7100623810 (+0x340), it records a bounce (owner position, reversed direction,
// angle) and sends it out through two virtual hooks; anything else goes to the base test. Names and
// layouts are placeholders.
#include <math.h>

#include "base/global/CFilePathStrId.h"
#include "game/CProjectileMovement.h"
#include "game/CVulkranMagmaBallMovementComponent.h"
#include "game/CWeaponMovement.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

bool fn_71006dfa88(UnkVulkranMagmaBall* self, UnkActor* other, void* a2, void* a3, void* a4, void* a5) {
    if (!other)
        return false;
    UnkMagmaBallHolder* holder = self->holder;
    if ((holder ? holder->actor : nullptr) == other)
        return false;
    if (reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310)) != other ||
        fn_7100ac79d8(self, other, a4))
        return fn_7100ac4ec0(reinterpret_cast<UnkObj*>(self), other, a2, a3, a4, a5);
    UnkMagmaBallPlayerPart* part = fn_7100623810(other);
    if (!part || !part->armed)
        return fn_7100ac4ec0(reinterpret_cast<UnkObj*>(self), other, a2, a3, a4, a5);

    f32 vx = self->dir_x;
    UnkActor* owner = self->owner;
    f32 px = owner->field_0x44;
    f32 py = owner->field_0x54;
    f32 vy = self->dir_y;
    f32 vz = self->dir_z;
    f32 pz = owner->field_0x64;
    f32 length = sqrtf(vx * vx + vy * vy + vz * vz);
    self->shot_id = -1;
    UnkMagmaBallShot& shot = self->shot;
    f32 inv = 1.0f / (length + 0.0001f);
    shot.pos.x = px;
    shot.pos.y = py;
    shot.pos.z = pz;
    shot.dir.x = -(vx * inv);
    shot.dir.y = -(vy * inv);
    shot.dir.z = -(vz * inv);
    shot.field_0x18 = 0.0f;
    shot.field_0x1c = 0.0f;
    shot.field_0x20 = 0.0f;
    shot.field_0x24 = 0.0f;
    shot.field_0x28 = 40;
    f32 heading = atan2f(shot.dir.y, shot.dir.x);
    UnkVec3f angles;
    angles.x = 0.0f;
    angles.y = 0.0f;
    angles.z = heading;
    self->vtbl->vf_0x338(self, &shot, &angles, g_unk_710164a808, &g_unk_7101d470e8);
    if (!self->def->no_hit)
        self->vtbl->vf_0x2a8(self, other, a4);
    return false;
}
