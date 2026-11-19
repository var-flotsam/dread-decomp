// CNailongThornMovementComponent's unit (tools/tu_map.py): the thorn touches `other`. Its holder's actor or
// a second touch is no hit. The player, not already handled (fn_7100ac79d8) with its part armed, gets a shot
// fired back along the thorn's direction (vf_0x338) and the hit (vf_0x2a8); anyone else takes the hit info's
// shot (fn_7100ac82c0), the owner's timeline gets "onimpact" and the base test runs. Names and layouts are
// placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CName.h"
#include "../../game_s2/UnkSpitSend.h"
#include "game/CProjectileMovement.h"
#include "game/CWeaponMovement.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkXformNode.h"
#include "types.h"

extern "C" f32 atan2f(f32 y, f32 x);
extern const char g_unk_710164a300[] __attribute__((visibility("hidden")));

struct UnkObj;

struct UnkMagmaBallPlayerPart_l6caaf8 {
    u8 pad_0x0[0x340];
    bool armed;  // 0x340
};
UnkMagmaBallPlayerPart_l6caaf8* fn_7100623810(UnkActor* actor);

struct UnkThornHolder_l6234e8 {
    u8 pad_0x0[0x10];
    UnkActor* actor;  // 0x10
};

struct UnkThornDef_l6234e8 {
    u8 pad_0x0[0xed];
    bool no_hit;  // 0xed
};

struct UnkThornVec_l6234e8 {
    f32 x, y, z;
};

// Where a shot goes and how it moves.
struct UnkThornShotPath_l6234e8 {
    UnkThornVec_l6234e8 pos;  // 0x0
    UnkThornVec_l6234e8 dir;  // 0xc
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    f32 field_0x24;
};

// What the shot carries (+0x3b4 of the movement).
struct UnkThornShot_l6234e8 {
    UnkThornShotPath_l6234e8 path;  // 0x0
    s32 field_0x28;
};

// The hit's description (the fourth argument).
struct UnkThornHitInfo_l6234e8 {
    u8 pad_0x0[0x14];
    UnkThornShotPath_l6234e8 path;  // 0x14
};

struct UnkThornMovement_l6234e8;

struct UnkThornMovementVtbl_l6234e8 {
    void* slots[0x2a8 / 8];
    void (*vf_0x2a8)(UnkThornMovement_l6234e8* self, UnkActor* other, void* info);
    void* slots_0x2b0[(0x338 - 0x2b0) / 8];
    void (*vf_0x338)(UnkThornMovement_l6234e8* self, UnkThornShot_l6234e8* shot, const UnkVec3f* angles,
                     const char* kind, base::global::CFilePathStrId* path);
};

struct UnkThornMovement_l6234e8 {
    UnkThornMovementVtbl_l6234e8* vtbl;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0xd4];
    f32 dir_x;  // 0x10c
    f32 dir_y;  // 0x110
    f32 dir_z;  // 0x114
    u8 pad_0x118[0x118];
    UnkThornHolder_l6234e8* holder;  // 0x230
    u8 pad_0x238[0x170];
    void* field_0x3a8;
    s32 shot_id;  // 0x3b0
    UnkThornShot_l6234e8 shot;  // 0x3b4
    u8 pad_0x3e0[0x1f0];
    UnkThornDef_l6234e8* def;  // 0x5d0
    bool done;  // 0x5d8
};
static_assert(__builtin_offsetof(UnkThornMovement_l6234e8, shot) == 0x3b4, "layout");
static_assert(__builtin_offsetof(UnkThornMovement_l6234e8, def) == 0x5d0, "layout");
static_assert(__builtin_offsetof(UnkThornMovement_l6234e8, done) == 0x5d8, "layout");

bool fn_71006234e8(UnkThornMovement_l6234e8* self, UnkActor* other, void* a2, void* a3,
                   UnkThornHitInfo_l6234e8* info, void* a5) {
    UnkThornHolder_l6234e8* holder = self->holder;
    if ((holder ? holder->actor : nullptr) == other || self->done)
        return false;
    self->done = true;
    if (reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310)) == other && !fn_7100ac79d8(self, other, info)) {
        UnkMagmaBallPlayerPart_l6caaf8* part = fn_7100623810(other);
        if (part && part->armed) {
            UnkActor* owner = self->owner;
            UnkThornVec_l6234e8 pos = {owner->field_0x44, owner->field_0x54, owner->field_0x64};
            f32 dx = self->dir_x, dy = self->dir_y, dz = self->dir_z;
            f32 len = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
            self->shot_id = -1;
            f32 inv = 1.0f / (len + 0.0001f);
            f32 nx = -(dx * inv), ny = -(dy * inv), nz = -(dz * inv);
            UnkThornShot_l6234e8* shot = &self->shot;
            shot->path.pos = pos;
            shot->path.dir.x = nx;
            shot->path.dir.y = ny;
            shot->path.dir.z = nz;
            shot->path.field_0x18 = 0.0f;
            shot->path.field_0x1c = 0.0f;
            shot->path.field_0x20 = 0.0f;
            shot->path.field_0x24 = 0.0f;
            shot->field_0x28 = 40;
            UnkVec3f angles(0.0f, 0.0f, atan2f(ny, nx));
            self->vtbl->vf_0x338(self, shot, &angles, g_unk_710164a300, &g_unk_7101d470e8);
            if (self->def->no_hit)
                return false;
            self->vtbl->vf_0x2a8(self, other, info);
            return false;
        }
    }
    if (other) {
        self->shot_id = -1;
        self->shot.path = info->path;
        self->field_0x3a8 = a5;
        self->shot.field_0x28 = 0x238;
        fn_7100ac82c0(reinterpret_cast<UnkSplash_b59a4*>(self), other);
    }
    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        base::reflection::UnkReflTypeIdArg mask;
        base::global::CName name("onimpact");
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &name.hash, &mask.id, false, -1.0f);
    }
    return fn_7100ac4ec0(reinterpret_cast<UnkObj*>(self), other, a2, a3, info, a5);
}
