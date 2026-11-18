// CVulkranMagmaBallMovementComponent's unit (tools/tu_map.py): the update. After the base projectile step
// (fn_7100ac3a78), when fn_71006df98c says so, fires a shot back from the owner's position against the ball's
// direction (vf_0x338). Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "game/CProjectileMovement.h"
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "types.h"

extern "C" f32 atan2f(f32 y, f32 x);
extern const char g_unk_710164a808[] __attribute__((visibility("hidden")));

// What the shot carries (+0x3b4 of the movement).
struct UnkVulkranMagmaBallShot_l6df868 {
    UnkVec3f pos;  // 0x0
    UnkVec3f dir;  // 0xc
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    f32 field_0x24;
    s32 field_0x28;
};

struct UnkVulkranMagmaBall_l6df868;

struct UnkVulkranMagmaBallVtbl_l6df868 {
    void* slots[0x338 / 8];
    void (*vf_0x338)(UnkVulkranMagmaBall_l6df868* self, UnkVulkranMagmaBallShot_l6df868* shot,
                     const UnkVec3f* angles, const char* kind, base::global::CFilePathStrId* path);
};

struct UnkVulkranMagmaBall_l6df868 {
    UnkVulkranMagmaBallVtbl_l6df868* vtbl;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0xd4];
    f32 dir_x;  // 0x10c
    f32 dir_y;  // 0x110
    f32 dir_z;  // 0x114
    u8 pad_0x118[0x298];
    s32 shot_id;                           // 0x3b0
    UnkVulkranMagmaBallShot_l6df868 shot;  // 0x3b4
};
static_assert(__builtin_offsetof(UnkVulkranMagmaBall_l6df868, shot) == 0x3b4, "layout");

struct UnkMagmaBall_b50;
bool fn_71006df98c(UnkMagmaBall_b50* self);

void fn_71006df868(UnkVulkranMagmaBall_l6df868* self, f32 dt) {
    fn_7100ac3a78(reinterpret_cast<UnkNailongThornMovement*>(self), dt);
    if (!fn_71006df98c(reinterpret_cast<UnkMagmaBall_b50*>(self)))
        return;
    UnkActor* owner = self->owner;
    UnkVec3f pos(owner->field_0x44, owner->field_0x54, owner->field_0x64);
    f32 dx = self->dir_x, dy = self->dir_y, dz = self->dir_z;
    f32 len = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    self->shot_id = -1;
    f32 inv = 1.0f / (len + 0.0001f);
    f32 nx = -(dx * inv), ny = -(dy * inv), nz = -(dz * inv);
    UnkVulkranMagmaBallShot_l6df868* shot = &self->shot;
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
    self->vtbl->vf_0x338(self, shot, &angles, g_unk_710164a808, &g_unk_7101d470e8);
}
