// CEmmyWaveMovementComponent unit: fn_7100502998 answers whether the player lies ahead of the movement's
// direction (+0x40): the 2D dot product of that direction and the owner-to-player offset is not negative.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkPlayer;

struct UnkWaveEntity_b62a3 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
};

struct UnkWaveMovement_b62a3 {
    u8 pad_0x0[0x30];
    UnkWaveEntity_b62a3* owner;
    u8 pad_0x38[0x8];
    base::math::CVector3D dir;
};

bool fn_7100502998(UnkWaveMovement_b62a3* self) {
    base::math::CVector2D dir;
    fn_710007b430(&dir, &self->dir);
    auto* player = reinterpret_cast<UnkWaveEntity_b62a3*>(fn_7100dec788(g_unk_7101d47310));
    UnkWaveEntity_b62a3* owner = self->owner;
    base::math::CVector2D off;
    f32 px = player->x, py = player->y, pz = player->z;
    f32 ox = owner->x, oy = owner->y, oz = owner->z;
    base::math::CVector3D d = {px - ox, py - oy, pz - oz};
    fn_710007b430(&off, &d);
    return !(dir.x * off.x + dir.y * off.y < 0.0f);
}
