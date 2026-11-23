// CSpittailMagmaBallMovementComponent's unit (tools/tu_map.py): the ball touches `other`. Ignores none
// and its own holder's actor; unless already done (+0x5d8), runs fn_71006ca5d4 for anyone but the holder's
// target. When `other` is the player, not already handled (fn_7100ac79d8) and its part (fn_7100623810) is
// armed, fires a shot back along the ball's direction (vf_0x338) and hits the player (vf_0x2a8, unless
// the definition's +0xed says not to); otherwise the base handles it. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "game/CSpittailMagmaBallMovementComponent.h"
#include "game/CVulkranMagmaBallMovementComponent.h"
#include "game/CWeaponMovement.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkXformNode.h"
#include "types.h"

extern "C" f32 atan2f(f32 y, f32 x);
extern const char g_unk_710164a788[] __attribute__((visibility("hidden")));

struct UnkSpittailMagmaBallMovement;
// Whether the hit was already handled (bombs that touch the player or something they ignore).

// The Spittail ball: the Vulkran magma ball's movement and what it adds.
struct UnkMagmaBallMovement_l6caaf8 : UnkVulkranMagmaBall {
    bool done;  // 0x5d8
    u8 pad_0x5d9[7];
    UnkMagmaBallDef* spittail_def;  // 0x5e0
};
static_assert(__builtin_offsetof(UnkMagmaBallMovement_l6caaf8, spittail_def) == 0x5e0, "layout");

bool fn_71006caaf8(UnkMagmaBallMovement_l6caaf8* self, UnkActor* other, void* a2, void* a3, void* a4,
                   void* a5) {
    if (!other)
        return false;
    UnkMagmaBallHolder* holder = self->holder;
    if ((holder ? holder->actor : nullptr) == other)
        return false;
    if (!self->done && !(holder && holder->actor && fn_7100d7c990(holder->actor) == other))
        fn_71006ca5d4(reinterpret_cast<UnkSpittailMagmaBallMovement*>(self), a4);
    if (reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310)) != other || fn_7100ac79d8(self, other, a4))
        return fn_71006dfa88(self, other, a2, a3, a4, a5);
    UnkMagmaBallPlayerPart* part = fn_7100623810(other);
    if (!part || !part->armed)
        return fn_71006dfa88(self, other, a2, a3, a4, a5);
    UnkActor* owner = self->owner;
    UnkVec3f pos(owner->field_0x44, owner->field_0x54, owner->field_0x64);
    f32 dx = self->dir_x, dy = self->dir_y, dz = self->dir_z;
    f32 len = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    self->shot_id = -1;
    f32 inv = 1.0f / (len + 0.0001f);
    f32 nx = -(dx * inv), ny = -(dy * inv), nz = -(dz * inv);
    UnkMagmaBallShot* shot = &self->shot;
    shot->pos = pos;
    shot->dir.x = nx;
    shot->dir.y = ny;
    shot->dir.z = nz;
    shot->field_0x18 = 0.0f;
    shot->field_0x1c = 0.0f;
    shot->field_0x20 = 0.0f;
    shot->field_0x24 = 0.0f;
    shot->field_0x28 = 40;
    UnkVec3f angles(0.0f, 0.0f, atan2f(ny, nx));
    self->vtbl->vf_0x338(self, shot, &angles, g_unk_710164a788, &g_unk_7101d470e8);
    if (!self->spittail_def->no_hit)
        self->vtbl->vf_0x2a8(self, other, a4);
    return false;
}
