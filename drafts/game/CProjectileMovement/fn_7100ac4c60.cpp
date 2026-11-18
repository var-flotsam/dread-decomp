// CProjectileMovement unit (tools/tu_map.py): the base hit handler. Fires the follow-up shot (vf_0x338)
// from the hit position along the hit normal, nudged by the linked object's (+0x230) drift when the
// normal is near vertical, moves the owner there and, when the definition says so, marks the
// projectile done. Always returns true. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "game/CProjectileMovement.h"
#include "game/UnkXformNode.h"
#include "types.h"

extern "C" f32 atan2f(f32 y, f32 x);
extern const u8 g_unk_710164b89c[] __attribute__((visibility("hidden")));

struct UnkProjectileHitInfo_lac4c60 {
    u8 pad_0x0[0x14];
    UnkVec3f pos;     // 0x14
    u8 pad_0x20[0xc];
    UnkVec3f normal;  // 0x2c
};

struct UnkDriftSource_lac4c60 {
    u8 pad_0x0[0x10c];
    f32 drift_x;  // 0x10c
    u8 pad_0x110[4];
    f32 drift_z;  // 0x114
};

struct UnkLinkedActor_lac4c60 {
    u8 pad_0x0[0x1f0];
    UnkDriftSource_lac4c60* drift;  // 0x1f0
};

struct UnkLink_lac4c60 {
    u8 pad_0x0[0x10];
    UnkLinkedActor_lac4c60* actor;  // 0x10
};

struct UnkProjectileOwner_lac4c60 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x3b0 - 0x68];
    bool moved;  // 0x3b0
};

struct UnkProjectileDef_lac4c60 {
    u8 pad_0x0[0x131];
    bool stop_on_hit;  // 0x131
};

// What the shot carries (+0x3b4 of the movement).
struct UnkProjectileShot_lac4c60 {
    UnkVec3f pos;  // 0x0
    UnkVec3f dir;  // 0xc
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    f32 field_0x24;
    s32 field_0x28;
};

struct UnkProjectileMovement_lac4c60;

struct UnkProjectileMovementVtbl_lac4c60 {
    void* slots[0x338 / 8];
    void (*vf_0x338)(UnkProjectileMovement_lac4c60* self, UnkProjectileShot_lac4c60* shot, const UnkVec3f* angles,
                     const u8* kind, base::global::CFilePathStrId* path);
};

struct UnkProjectileMovement_lac4c60 {
    UnkProjectileMovementVtbl_lac4c60* vtbl;
    u8 pad_0x8[0x28];
    UnkProjectileOwner_lac4c60* owner;  // 0x30
    u8 pad_0x38[0x230 - 0x38];
    UnkLink_lac4c60* link;  // 0x230
    u8 pad_0x238[0x3b0 - 0x238];
    s32 shot_id;                       // 0x3b0
    UnkProjectileShot_lac4c60 shot;    // 0x3b4
    u8 pad_0x3e0[0x578 - 0x3e0];
    bool done;  // 0x578
    u8 pad_0x579[0x5c8 - 0x579];
    UnkProjectileDef_lac4c60* def;  // 0x5c8
};
static_assert(__builtin_offsetof(UnkProjectileMovement_lac4c60, shot) == 0x3b4, "layout");
static_assert(__builtin_offsetof(UnkProjectileMovement_lac4c60, def) == 0x5c8, "layout");

bool fn_7100ac4c60(UnkBossComponent* component, u64 a, u64 b, u64 c, u64 d) {
    auto* self = reinterpret_cast<UnkProjectileMovement_lac4c60*>(component);
    const auto* hit = reinterpret_cast<const UnkProjectileHitInfo_lac4c60*>(c);
    f32 nx = hit->normal.x, ny = hit->normal.y, nz = hit->normal.z;
    UnkVec3f pos = hit->pos;
    if (__builtin_fabsf(nx * 0.0f + ny + nz * 0.0f) > 0.7f) {
        UnkLink_lac4c60* link = self->link;
        if (link && link->actor && link->actor->drift) {
            UnkDriftSource_lac4c60* drift = link->actor->drift;
            f32 dx = drift->drift_x * 0.1f;
            f32 dz = drift->drift_z * 0.1f;
            pos.x += dx;
            pos.y += 0.0f;
            pos.z += dz;
        }
    }
    self->shot_id = -1;
    UnkProjectileShot_lac4c60* shot = &self->shot;
    shot->pos = pos;
    shot->dir.x = nx;
    shot->dir.y = ny;
    shot->dir.z = nz;
    shot->field_0x18 = 0.0f;
    shot->field_0x1c = 0.0f;
    shot->field_0x20 = 0.0f;
    shot->field_0x24 = 0.0f;
    shot->field_0x28 = 40;
    UnkProjectileOwner_lac4c60* owner = self->owner;
    owner->x = pos.x;
    owner->y = pos.y;
    owner->z = pos.z;
    owner->moved = true;
    UnkVec3f angles(0.0f, 0.0f, atan2f(shot->dir.y, shot->dir.x));
    self->vtbl->vf_0x338(self, shot, &angles, g_unk_710164b89c, &g_unk_7101d470e8);
    if (self->def->stop_on_hit)
        self->done = true;
    return true;
}
