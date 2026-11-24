// CChozoCommanderAIComponent unit: the commander's melee step. Times the current animation entry's approach,
// sends the melee success (or fall success) action to the target, points the melee camera at it, and on a
// Ki counter attack the target's melee answers turns the melee camera. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAnimationComponent.h"
#include "game/CCameraComponent.h"
#include "game/CMeleeComponent.h"
#include "game/UnkActor.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "game/UnkTU1730.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;

struct UnkCls_710117af10;
struct UnkStruct_710117af10;
struct UnkStruct_7101176ea8;
struct UnkSyncBlockCameraTarget;
struct UnkWeakTarget;
struct UnkCameraOwner_l3530d4;
struct UnkDesc_SpitSend;

extern UnkDesc_SpitSend g_unk_710164b958;
extern __attribute__((visibility("hidden"))) UnkCls_710117af10* g_unk_71726c9508;

void fn_710110ca70(void* self);
void CSynchronizedBlockCameraCtrl_SetTarget(UnkSyncBlockCameraTarget* self, UnkWeakTarget* target);
void fn_71003530d4(UnkCameraOwner_l3530d4* self);

struct UnkFlags_l308950 {
    u32 bits;
    UnkFlags_l308950() : bits(0) {}
    UnkFlags_l308950(u32 b) : bits(b) {}
    UnkFlags_l308950(const UnkFlags_l308950& o) : bits(o.bits) {}
};

inline const UnkFlags_l308950& UnkFlagsNone_l308950() {
    static UnkFlags_l308950 s_none asm("g_unk_71726a4340");
    return s_none;
}

inline const UnkFlags_l308950& UnkFlagsAll_l308950() {
    static UnkFlags_l308950 s_all asm("g_unk_71726a4330")(~UnkFlagsNone_l308950().bits & 7);
    return s_all;
}

struct UnkStruct_71007142e8 {
    f32 m[12];
    u8 pad_0x30[0x40];
};

struct UnkMotionTracks;
struct UnkTimeline {
    u8 pad_0x0[0x24];
    s32 frame_count;
    u8 pad_0x28[0x58];
    UnkMotionTracks* tracks;  // 0x80
};

inline const CStrId& MeleeCameraId_l45332c() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return *reinterpret_cast<const CStrId*>(reinterpret_cast<const u8*>(&s_table) + 0x1780);
}

// The animation key the approach is timed against.
struct UnkAnimKey_l45332c {
    u8 pad_0x0[0x30];
    void* field_0x30;
};

struct UnkReflObject_l45332c {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkReflObject_l45332c*)>(vtable[0])(this); }
};

struct UnkMeleeState_l45332c {
    u8 pad_0x0[0xf9];
    bool falling;  // 0xf9
};

struct UnkMeleeTarget_l45332c {
    u8 pad_0x0[0x1f0];
    UnkMeleeState_l45332c* melee_state;  // 0x1f0
    u8 pad_0x1f8[0x28];
    void* animation;  // 0x220
    u8 pad_0x228[0xd0];
    void* melee;  // 0x2f8
};

struct UnkMeleeParams_l45332c {
    u8 pad_0x0[0x144];
    f32 field_0x144;
    f32 field_0x148;
    u8 pad_0x14c[0x2e1];
    bool field_0x42d;
};

// The descriptor the fall success is sent with.
struct UnkBlendDesc_l45332c {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
};

struct UnkMeleeCameraCtrl_l45332c : UnkCameraCtrl {
    u8 pad_0x28[0x298];
    CStrId focus;     // 0x2c0
    f32 dir_x;        // 0x2c8
    f32 dir_y;
    f32 dir_z;
    f32 field_0x2d4;
    f32 field_0x2d8;
    u8 pad_0x2dc[4];
    CStrId blend;     // 0x2e0
    u8 pad_0x2e8[4];
    f32 near_x;       // 0x2ec
    f32 near_y;
    f32 near_z;
    f32 far_x;        // 0x2f8
    f32 far_y;
    f32 far_z;
    f32 field_0x304;
    f32 field_0x308;
    u8 pad_0x30c[0x24];
    bool field_0x330;

    void SetFocus(const CStrId& id, f32 x, f32 y, f32 z) {
        focus = id;
        dir_x = x;
        dir_y = y;
        dir_z = z;
    }
    void SetBlend(const CStrId& id, f32 a, f32 b) {
        field_0x2d4 = a;
        field_0x2d8 = b;
        blend = id;
    }
    void SetBox() {
        near_x = 100.0f;
        near_y = 50.0f;
        near_z = 0.0f;
        far_x = 100.0f;
        far_y = 50.0f;
        far_z = 0.0f;
    }
};
static_assert(__builtin_offsetof(UnkMeleeCameraCtrl_l45332c, field_0x330) == 0x330, "melee camera layout");

struct UnkCommanderAI_l45332c {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x1110];
    UnkReflObject_l45332c* attack;  // 0x1148
    u8 pad_0x1150[0x13850];
    UnkMeleeParams_l45332c* params;  // 0x149a0
    u8 pad_0x149a8[0x135f];
    bool countered;  // 0x15d07
    u8 pad_0x15d08[0x1dc];
    f32 approach_time;  // 0x15ee4
};
static_assert(__builtin_offsetof(UnkCommanderAI_l45332c, approach_time) == 0x15ee4, "commander layout");

struct UnkVec3_l45332c {
    f32 x;
    f32 y;
    f32 z;
};

static inline UnkVec3_l45332c operator-(const UnkVec3_l45332c& a, const UnkVec3_l45332c& b) {
    UnkVec3_l45332c r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

// An actor's position (the translation of its transform).
static inline UnkVec3_l45332c Pos(const UnkActor* actor) {
    UnkVec3_l45332c p;
    p.x = actor->field_0x44;
    p.y = actor->field_0x54;
    p.z = actor->field_0x64;
    return p;
}

static const u64 kKindU32 = 0xf4061ff25a2163d6;

// The melee camera the holder has under the melee id, when it is one.
static inline UnkMeleeCameraCtrl_l45332c* MeleeCamera(void* holder) {
    UnkCameraCtrl* ctrl = fn_71010e389c(holder, const_cast<CStrId*>(&MeleeCameraId_l45332c()));
    if (!ctrl || ctrl->vf_0x0() != fn_7100927674())
        return nullptr;
    return static_cast<UnkMeleeCameraCtrl_l45332c*>(ctrl);
}

static inline bool IsMeleeCamera(UnkCameraCtrl* ctrl) {
    return fn_710009d50c(ctrl->vf_0x0(), fn_7100927674());
}

// The z of the translation the timeline's motion makes over its whole duration.
static inline f32 MotionZ(UnkTimeline* timeline) {
    UnkStruct_71007142e8 motion;
    fn_71007142e8(&motion);
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_l308950 flags = UnkFlagsAll_l308950();
    fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
    return motion.m[11];
}

void fn_710045332c(UnkCommanderAI_l45332c* self) {
    using base::reflection::UnkReflTypeId;
    using base::reflection::UnkReflTypeIdArg;
    using base::reflection::UnkReflTypeIdTag;
    UnkReflTypeIdArg success_mode;
    UnkReflTypeIdArg success_id;
    UnkReflTypeIdArg fall_mode;
    UnkReflTypeIdArg fall_id;
    fn_710110ca70(self);
    void* anim = self->owner->field_0x220;
    if (anim && CAnimationComponent_IsPlayingActionNameContaining(static_cast<UnkStruct_710117af10*>(anim), &g_unk_71726c9508, false)) {
        auto* entry = static_cast<UnkTimeline*>(
            fn_7101176d34(static_cast<UnkStruct_7101176ea8*>(self->owner->field_0x220),
                          reinterpret_cast<UnkAnimKey_l45332c*>(g_unk_71726c9508)->field_0x30));
        if (entry) {
            f32* approach = &self->approach_time;
            f32 speed = fabsf(MotionZ(entry));
            f32 facing_x = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38)).m[2];
            f32 facing = (f32)((facing_x > 0.0f) - (facing_x < 0.0f));
            f32 x = self->owner->field_0x44;
            f32 target_x = fn_7100d7c990(self->owner)->field_0x44;
            *approach = ((x - target_x) * facing + 600.0f) / speed;
        }

        UnkActor* target_actor = fn_7100d7c990(self->owner);
        auto* target = reinterpret_cast<UnkMeleeTarget_l45332c*>(target_actor);
        void* animation = target ? target->animation : nullptr;
        if (animation && target->melee_state) {
            if (target->melee_state->falling) {
                u64 hash = StringCrc64("meleefallsuccess", 16);
                fall_mode.id = 1;
                UnkBlendDesc_l45332c desc;
                desc.field_0x0 = 0.0f;
                desc.field_0x4 = 0.0f;
                desc.field_0x8 = 4.0f;
                fall_id.id = UnkReflTypeId<UnkReflTypeIdTag>::id();
                CAnimationComponent_SendActionToTrack(animation, hash, kKindU32, &fall_mode, &g_unk_710164b958, &desc, &fall_id);
                void* holder = fn_7100defcec(g_unk_7101d47310);
                if (holder) {
                    UnkCameraCtrl* current = fn_71010e2080(holder);
                    if (current && IsMeleeCamera(current))
                        fn_71003530d4(static_cast<UnkCameraOwner_l3530d4*>(holder));
                    UnkMeleeCameraCtrl_l45332c* camera = MeleeCamera(holder);
                    if (camera) {
                        CSynchronizedBlockCameraCtrl_SetTarget(reinterpret_cast<UnkSyncBlockCameraTarget*>(camera),
                                      reinterpret_cast<UnkWeakTarget*>(self->owner));
                        camera->field_0x304 = self->params->field_0x148;
                        camera->field_0x330 = self->params->field_0x42d;
                        camera->SetBox();
                        camera->field_0x308 = self->params->field_0x144;
                        UnkVec3_l45332c d = Pos(self->owner) - Pos(target_actor);
                        f32 dx = d.x;
                        f32 dy = d.y;
                        f32 dz = d.z;
                        f32 scale = 1.0f / (sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
                        dx *= scale;
                        dy *= scale;
                        dz *= scale;
                        camera->SetFocus(CStrId("MELEE", true), dx, dy, dz);
                        camera->SetBlend(CStrId("diss", true), 0.33f, 0.01f);
                        fn_71010e4168(holder, &MeleeCameraId_l45332c(), false);
                    }
                }
            } else {
                u64 hash = StringCrc64("meleesuccess_commander", 22);
                success_mode.id = 1;
                success_id.id = UnkReflTypeId<UnkReflTypeIdTag>::id();
                CAnimationComponent_SendActionToTrack(animation, hash, kKindU32, &success_mode, &g_unk_710164b958, &g_unk_7172769740,
                              &success_id);
            }
        }
    }

    UnkReflObject_l45332c* attack = self->attack;
    if (!attack || !fn_710009d50c(attack->vf_0x0(), fn_710042e3d4()))
        return;
    auto* answer = reinterpret_cast<UnkMeleeTarget_l45332c*>(fn_7100d7c990(self->owner));
    if (!answer || !answer->melee || !fn_710092b6f8(answer->melee))
        return;
    self->countered = true;
    static CStrId s_counter("MELEE_COUNTER", true);
    static CStrId s_turn("qz2si", true);
    void* holder = fn_7100defcec(g_unk_7101d47310);
    if (!holder)
        return;
    UnkCameraCtrl* current = fn_71010e2080(holder);
    if (current && IsMeleeCamera(current))
        return;
    UnkMeleeCameraCtrl_l45332c* camera = MeleeCamera(holder);
    if (!camera)
        return;
    CSynchronizedBlockCameraCtrl_SetTarget(reinterpret_cast<UnkSyncBlockCameraTarget*>(camera), reinterpret_cast<UnkWeakTarget*>(self->owner));
    camera->field_0x304 = 0.5f;
    camera->field_0x308 = 1200.0f;
    camera->SetBox();
    fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
    camera->SetBlend(s_turn, 0.6f, 0.08f);
    fn_71010e4168(holder, &MeleeCameraId_l45332c(), false);
}
