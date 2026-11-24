// CChozoWarriorAIComponent's unit (tools/tu_map.py): unless +0x14999 is set (then fn_7101108b98 runs), switches
// the game's camera to the melee camera: aimed along the owner's facing, with the FX preset an
// override track names (or "MELEE_COUNTER") and the "diss" effect. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCameraComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;

struct UnkSyncBlockCameraTarget;
struct UnkWeakTarget;
struct UnkImpactAI_l108b98;
void CSynchronizedBlockCameraCtrl_SetTarget(UnkSyncBlockCameraTarget* self, UnkWeakTarget* target);
void fn_7101108b98(UnkImpactAI_l108b98* self);

// The melee camera's id: the slot at +0x1780 of the game's id table, which the table's header has not
// laid out yet.
inline const CStrId& MeleeCameraId_l49a664() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return *reinterpret_cast<const CStrId*>(reinterpret_cast<const char*>(&s_table) + 0x1780);
}

// A CMeleeCameraCtrl: its FX preset, the direction it looks along and its tuning.
struct UnkMeleeCameraCtrl_l49a664 {
    void** vtable;
    u8 pad_0x8[0x2c0 - 0x8];
    CStrId preset;  // 0x2c0
    CVector3D dir;  // 0x2c8
    f32 field_0x2d4;
    f32 field_0x2d8;
    u8 pad_0x2dc[0x2e0 - 0x2dc];
    CStrId effect;  // 0x2e0
    u8 pad_0x2e8[0x2ec - 0x2e8];
    f32 field_0x2ec;
    f32 field_0x2f0;
    f32 field_0x2f4;
    f32 field_0x2f8;
    f32 field_0x2fc;
    f32 field_0x300;
    f32 field_0x304;
    f32 field_0x308;
};
static_assert(__builtin_offsetof(UnkMeleeCameraCtrl_l49a664, field_0x308) == 0x308, "");

// The transform's z axis, normalised.
static inline CVector3D Forward(const UnkMatrix44& m) {
    CVector3D v;
    v.x = m.m[2];
    v.y = m.m[6];
    v.z = m.m[10];
    f32 inv = 1.0f / (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z) + 0.0001f);
    v.x *= inv;
    v.y *= inv;
    v.z *= inv;
    return v;
}

struct UnkChozoWarriorOwner_l49a664 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x220 - 0x78];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkChozoWarriorAI_l49a664 {
    u8 pad_0x0[0x30];
    UnkChozoWarriorOwner_l49a664* owner;  // 0x30
    u8 pad_0x38[0x14999 - 0x38];
    bool field_0x14999;
};

void fn_710049a664(UnkChozoWarriorAI_l49a664* self) {
    if (self->field_0x14999) {
        fn_7101108b98(reinterpret_cast<UnkImpactAI_l108b98*>(self));
        return;
    }
    static CStrId s_counter("MELEE_COUNTER", true);
    static CStrId s_diss("diss", true);
    void* holder = fn_7100defcec(g_unk_7101d47310);
    if (!holder)
        return;
    UnkCameraCtrl* current = fn_71010e2080(holder);
    if (current && fn_710009d50c(current->vf_0x0(), fn_7100927674()))
        return;
    UnkCameraCtrl* found = fn_71010e389c(holder, const_cast<CStrId*>(&MeleeCameraId_l49a664()));
    if (!found || found->vf_0x0() != fn_7100927674())
        return;
    auto* ctrl = reinterpret_cast<UnkMeleeCameraCtrl_l49a664*>(found);
    CSynchronizedBlockCameraCtrl_SetTarget(reinterpret_cast<UnkSyncBlockCameraTarget*>(ctrl),
                  reinterpret_cast<UnkWeakTarget*>(self->owner));
    ctrl->field_0x2ec = 100.0f;
    ctrl->field_0x2f0 = 50.0f;
    ctrl->field_0x2f4 = 0.0f;
    ctrl->field_0x2f8 = 100.0f;
    ctrl->field_0x2fc = 50.0f;
    ctrl->field_0x300 = 0.0f;
    ctrl->field_0x304 = 0.5f;
    ctrl->field_0x308 = 1800.0f;
    CVector3D dir = Forward(fn_710002614c(&self->owner->transform));
    UnkTrackQuery query;
    query.cls = fn_7100d233f8();
    query.result = nullptr;
    query.value = -1.0f;
    if (UnkAnimationComponent* animation = self->owner->animation)
        fn_710117b08c(animation, &query);
    ctrl->preset = query.result ? query.result->sources : s_counter;
    ctrl->dir = dir;
    ctrl->field_0x2d4 = 0.5f;
    ctrl->field_0x2d8 = 0.01f;
    ctrl->effect = s_diss;
    fn_71010e4168(holder, &MeleeCameraId_l49a664(), false);
}
