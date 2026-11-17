// CNailuggerAcidBallMovementComponent's unit (tools/tu_map.py unit 943): the acid ball's hit test. Its own
// target and repeat hits are no hit; a hit on the player that the player's guard (+0x340) turns away is
// bounced back along the ball's velocity; any other hit is recorded, tells a Nailong AI, sends "onimpact" to
// the owner's timeline and goes to the base test. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CName.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CProjectileMovement.h"
#include "game/CWeaponMovement.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

#include <math.h>

struct UnkObj;
struct UnkSplash_b59a4;

bool fn_7100ac79d8(void* self, void* actor, void* info);

struct UnkAcidBallGuard_l626914 {
    u8 pad_0x0[0x340];
    bool active;  // 0x340
};

UnkAcidBallGuard_l626914* fn_7100623810(UnkActor* actor);

struct UnkAcidBallSpawnKey_l626914;
extern const UnkAcidBallSpawnKey_l626914 g_unk_710164a30c __attribute__((visibility("hidden")));

struct UnkAcidBallVec_l626914 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkAcidBallHitValues_l626914 {
    UnkAcidBallVec_l626914 pos;
    f32 dir[3];
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
};

struct UnkAcidBallHitInfo_l626914 {
    u8 pad_0x0[0x14];
    UnkAcidBallHitValues_l626914 values;  // 0x14
    u32 extra;                            // 0x38
};

struct UnkAcidBallHit_l626914 {
    s32 id;
    UnkAcidBallHitValues_l626914 values;
    u32 extra;
    u32 kind;
};

struct UnkAcidBallRotation_l626914 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkAcidBallListener_l626914;

struct UnkAcidBallListenerVtbl_l626914 {
    void* slots[0x90 / 8];
    void (*vf_0x90)(UnkAcidBallListener_l626914* self, UnkActor* owner, UnkActor* other);
};

struct UnkAcidBallListener_l626914 {
    UnkAcidBallListenerVtbl_l626914* vtbl;
};

struct UnkAcidBallAI_l626914 {
    virtual const base::reflection::CType* vf_0x0();
    u8 pad_0x8[0x146c8 - 0x8];
    UnkAcidBallListener_l626914 listener;  // 0x146c8
};

struct UnkAcidBallLink_l626914 {
    u8 pad_0x0[0x10];
    UnkActor* actor;  // 0x10
};

struct UnkAcidBallSettings_l626914 {
    u8 pad_0x0[0xed];
    bool field_0xed;
};

struct UnkAcidBall_l626914;

struct UnkAcidBallVtbl_l626914 {
    void* slots[0x2a8 / 8];
    void (*vf_0x2a8)(UnkAcidBall_l626914* self, UnkActor* other, UnkAcidBallHitInfo_l626914* info);
    void* slots_0x2b0[(0x338 - 0x2b0) / 8];
    void (*vf_0x338)(UnkAcidBall_l626914* self, const UnkAcidBallHitValues_l626914* values,
                     const UnkAcidBallRotation_l626914* rotation, const UnkAcidBallSpawnKey_l626914* key,
                     const base::global::CFilePathStrId* path);
};

struct UnkAcidBall_l626914 {
    UnkAcidBallVtbl_l626914* vtbl;
    u8 pad_0x8[0x30 - 0x8];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x10c - 0x38];
    f32 velocity[3];  // 0x10c
    u8 pad_0x118[0x230 - 0x118];
    UnkAcidBallLink_l626914* link;  // 0x230
    u8 pad_0x238[0x3b0 - 0x238];
    UnkAcidBallHit_l626914 hit;  // 0x3b0
    u8 pad_0x3e0[0x5d0 - 0x3e0];
    UnkAcidBallSettings_l626914* settings;  // 0x5d0
    bool hit_done;                          // 0x5d8
};

bool fn_7100626914(UnkAcidBall_l626914* self, UnkActor* other, void* a2, void* a3, UnkAcidBallHitInfo_l626914* info,
                   void* a5) {
    UnkActor* target = self->link ? self->link->actor : nullptr;
    if (target == other || self->hit_done)
        return false;
    self->hit_done = true;
    if (reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310)) == other && !fn_7100ac79d8(self, other, info)) {
        UnkAcidBallGuard_l626914* guard = fn_7100623810(other);
        if (guard && guard->active) {
            f32 vx = self->velocity[0];
            f32 vy = self->velocity[1];
            f32 vz = self->velocity[2];
            f32 lenSq = vx * vx + vy * vy + vz * vz;
            UnkActor* owner = self->owner;
            UnkAcidBallVec_l626914 pos = {owner->field_0x44, owner->field_0x54, owner->field_0x64};
            f32 len = __builtin_sqrtf(lenSq);
            f32 inv = 1.0f / (len + 0.0001f);
            f32 dx = -(vx * inv);
            f32 dy = -(vy * inv);
            f32 dz = -(vz * inv);
            self->hit.id = -1;
            self->hit.values.pos = pos;
            self->hit.values.dir[0] = dx;
            self->hit.values.dir[1] = dy;
            self->hit.values.dir[2] = dz;
            self->hit.values.field_0x18 = 0;
            self->hit.values.field_0x1c = 0;
            self->hit.values.field_0x20 = 0;
            self->hit.extra = 0;
            self->hit.kind = 0x28;
            UnkAcidBallRotation_l626914 rotation;
            rotation.x = 0.0f;
            rotation.y = 0.0f;
            rotation.z = atan2f(dy, dx);
            self->vtbl->vf_0x338(self, &self->hit.values, &rotation, &g_unk_710164a30c, &g_unk_7101d470e8);
            if (!self->settings->field_0xed)
                self->vtbl->vf_0x2a8(self, other, info);
            return false;
        }
    }
    if (other) {
        self->hit.id = -1;
        self->hit.values = info->values;
        self->hit.extra = info->extra;
        self->hit.kind = 0x238;
        if (fn_7100ac82c0(reinterpret_cast<UnkSplash_b59a4*>(self), other) == 1) {
            if (UnkAcidBallLink_l626914* link = self->link) {
                if (UnkActor* actor = link->actor) {
                    if (auto* ai = static_cast<UnkAcidBallAI_l626914*>(actor->field_0x240)) {
                        if (fn_710009d50c(ai->vf_0x0(), fn_710061f464()))
                            ai->listener.vtbl->vf_0x90(&ai->listener, self->owner, other);
                    }
                }
            }
        }
    }
    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        base::reflection::UnkReflTypeIdArg mask;
        base::global::CName name("onimpact");
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &name.hash, &mask.id, false, -1.0f);
    }
    return fn_7100ac4ec0(reinterpret_cast<UnkObj*>(self), other, a2, a3, info, a5);
}
