// Unit 437 (CAimComponent): takes the melee target from `owner` and, when asked, points the melee camera
// at it: in the melee's second state through the camera effects (the preset the owner's
// CCharClassOverrideMeleeCameraFXPresetTrack names), otherwise by setting up and starting the
// CMeleeCameraCtrl. Names and layouts are placeholders.
#include <math.h>

#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCameraComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU437.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

struct UnkCameraOwner_l3530d4;
void fn_71003530d4(UnkCameraOwner_l3530d4* self);
struct UnkSyncBlockCameraTarget;
void fn_7100a20d7c(UnkSyncBlockCameraTarget* self, UnkWeakTarget* target);

extern CStrId g_unk_71726b16c0 __attribute__((visibility("hidden")));  // the default preset
extern CStrId g_unk_71726b16c8 __attribute__((visibility("hidden")));

// A direction (no user copy: it stays in registers).
struct UnkMeleeDir_l352b14 {
    f32 x, y, z;
};

struct UnkMeleeEntity_l352b14 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

// A counted reference to the target.
struct UnkMeleeHandle_l352b14 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // frees it
    s32 ref_count;  // 0x8
    UnkMeleeEntity_l352b14* entity;  // 0x10
};

struct UnkMeleeConfig_l352b14 {
    u8 pad_0x0[0x144];
    u32 field_0x144;
    u32 field_0x148;
    u8 pad_0x14c[0x2e1];
    u8 field_0x42d;
};
static_assert(__builtin_offsetof(UnkMeleeConfig_l352b14, field_0x42d) == 0x42d, "layout");

struct UnkMelee_l352b14 {
    virtual s32 vf_0x0();
    s32 State() {
        return reinterpret_cast<s32 (*)(UnkMelee_l352b14*)>((*reinterpret_cast<void***>(this))[0x520 / 8])(this);
    }
    bool IsBusy() {
        return reinterpret_cast<bool (*)(UnkMelee_l352b14*)>((*reinterpret_cast<void***>(this))[0x630 / 8])(this);
    }
    u8 pad_0x8[0x1608];
    UnkMeleeConfig_l352b14* config;  // 0x1610
};
static_assert(__builtin_offsetof(UnkMelee_l352b14, config) == 0x1610, "layout");

struct UnkMeleeTargetEntity_l352b14 {
    u8 pad_0x0[0x240];
    UnkMelee_l352b14* melee;  // 0x240
};

struct UnkMeleeSource_l352b14 {
    u8 pad_0x0[0x1b8];
    void* field_0x1b8;
};

struct UnkMeleeOwner_l352b14 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x18c];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkMeleeCamera_l352b14 : UnkCameraCtrl {
    u8 pad_0x28[0x298];
    CStrId preset;        // 0x2c0
    UnkVec3f direction;   // 0x2c8
    f32 field_0x2d4;
    f32 field_0x2d8;
    u8 pad_0x2dc[0x4];
    CStrId field_0x2e0;   // 0x2e0
    u8 pad_0x2e8[0x4];
    UnkVec3f field_0x2ec;  // 0x2ec
    UnkVec3f field_0x2f8;  // 0x2f8
    u32 field_0x304;
    u32 field_0x308;
    u8 pad_0x30c[0x24];
    u8 field_0x330;
};
static_assert(__builtin_offsetof(UnkMeleeCamera_l352b14, preset) == 0x2c0, "layout");
static_assert(__builtin_offsetof(UnkMeleeCamera_l352b14, field_0x2ec) == 0x2ec, "layout");
static_assert(__builtin_offsetof(UnkMeleeCamera_l352b14, field_0x330) == 0x330, "layout");

struct UnkMeleePresetTrack_l352b14 {
    u8 pad_0x0[0x18];
    CStrId preset;  // 0x18
};

// The id table's slot at +0x1780 (UnkGameManagerIdTable does not name it yet).
struct UnkIdTableSlot1780_l352b14 {
    u8 pad_0x0[0x1780];
    CStrId field_0x1780;
};

struct UnkMeleeAim_l352b14 {
    u8 pad_0x0[0x30];
    UnkMeleeOwner_l352b14* owner;  // 0x30
    u8 pad_0x38[0x648];
    UnkMeleeHandle_l352b14* target;  // 0x680

    void ResetTarget() {
        if (UnkMeleeHandle_l352b14* old = target) {
            if (--old->ref_count == 0)
                old->vf_0x10();
            target = nullptr;
        }
    }
    void SetTarget(UnkMeleeHandle_l352b14* handle) {
        if (target != handle) {
            if (handle)
                ++handle->ref_count;
            if (UnkMeleeHandle_l352b14* old = target) {
                if (--old->ref_count == 0)
                    old->vf_0x10();
            }
            target = handle;
        }
    }
    // The unit direction from the owner to the target.
    UnkMeleeDir_l352b14 TargetDirection() {
        UnkVec3f to = target->entity->xform.GetTranslation();
        UnkVec3f from = owner->xform.GetTranslation();
        f32 dx = to.x - from.x;
        f32 dy = to.y - from.y;
        f32 dz = to.z - from.z;
        f32 inv = 1.0f / (sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
        UnkMeleeDir_l352b14 dir = {dx * inv, dy * inv, dz * inv};
        return dir;
    }
    // The camera effects preset the owner's track names, or the default one.
    const CStrId* Preset(UnkTrackQuery* query) {
        if (UnkAnimationComponent* animation = owner->animation) {
            fn_710117b08c(animation, query);
            if (query->result)
                return &reinterpret_cast<UnkMeleePresetTrack_l352b14*>(query->result)->preset;
        }
        return &g_unk_71726b16c0;
    }
};
static_assert(__builtin_offsetof(UnkMeleeAim_l352b14, target) == 0x680, "layout");

static inline const CStrId& MeleeCameraId() {
    return reinterpret_cast<const UnkIdTableSlot1780_l352b14&>(GameManagerIdTable()).field_0x1780;
}

void fn_7100352b14(void* body_field, UnkActor* owner, u32 flag) {
    UnkMeleeAim_l352b14* self = static_cast<UnkMeleeAim_l352b14*>(body_field);
    self->ResetTarget();
    if (!owner || !reinterpret_cast<UnkMeleeSource_l352b14*>(owner)->field_0x1b8)
        return;
    self->SetTarget(reinterpret_cast<UnkMeleeHandle_l352b14*>(
        fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(owner))));
    UnkMeleeHandle_l352b14* handle = self->target;
    if (!handle || !handle->entity)
        return;
    UnkMelee_l352b14* melee = reinterpret_cast<UnkMeleeTargetEntity_l352b14*>(handle->entity)->melee;
    if (!melee || !(flag & 1))
        return;
    UnkMeleeConfig_l352b14* config = melee->config;
    if (!config)
        return;

    if (melee->State() != 0) {
        if (melee->State() != 2)
            return;
        UnkMeleeDir_l352b14 dir = self->TargetDirection();
        UnkTrackQuery query = {fn_7100d233f8(), nullptr, -1.0f};
        const CStrId* preset = self->Preset(&query);
        fn_7100e038b0(g_unk_7101d47310, const_cast<CStrId*>(preset), reinterpret_cast<UnkVec*>(&dir), 0);
        UnkGameManager* manager = g_unk_7101d47310;
        base::global::CRntString name(g_unk_71726b16c8.Text());
        fn_7100df6d3c(manager, &name, 0.01f, 0.33f);
        return;
    }

    if (melee->IsBusy())
        return;
    void* cameras = fn_7100defcec(g_unk_7101d47310);
    if (!cameras)
        return;
    UnkCameraCtrl* current = fn_71010e2080(cameras);
    if (current && fn_710009d50c(current->vf_0x0(), fn_7100927674()))
        fn_71003530d4(static_cast<UnkCameraOwner_l3530d4*>(cameras));
    UnkCameraCtrl* found = fn_71010e389c(cameras, const_cast<CStrId*>(&MeleeCameraId()));
    if (!found || found->vf_0x0() != fn_7100927674())
        return;
    UnkMeleeCamera_l352b14* camera = static_cast<UnkMeleeCamera_l352b14*>(found);
    fn_7100a20d7c(reinterpret_cast<UnkSyncBlockCameraTarget*>(camera),
                  self->target ? reinterpret_cast<UnkWeakTarget*>(self->target->entity) : nullptr);
    camera->field_0x304 = config->field_0x148;
    camera->field_0x330 = config->field_0x42d;
    camera->field_0x2ec = UnkVec3f(100.0f, 50.0f, 0.0f);
    camera->field_0x2f8 = UnkVec3f(100.0f, 50.0f, 0.0f);
    camera->field_0x308 = config->field_0x144;
    UnkMeleeDir_l352b14 dir = self->TargetDirection();
    UnkTrackQuery query = {fn_7100d233f8(), nullptr, -1.0f};
    camera->preset = *self->Preset(&query);
    camera->direction.x = dir.x;
    camera->direction.y = dir.y;
    camera->direction.z = dir.z;
    camera->field_0x2d4 = 0.33f;
    camera->field_0x2d8 = 0.01f;
    camera->field_0x2e0 = g_unk_71726b16c8;
    fn_71010e4168(cameras, &MeleeCameraId(), false);
}
