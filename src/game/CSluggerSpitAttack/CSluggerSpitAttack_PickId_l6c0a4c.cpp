// CSluggerSpitAttack unit: picks one of four ids by whether the animation allows it and whether the
// target lies behind the owner's facing (+0x8 column of its matrix). The empty id when there is no owner
// or no target. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

extern __attribute__((visibility("hidden"))) CStrId g_unk_7172707bd0[4];

struct UnkSluggerMovementData {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkSluggerMovementPart {
    u8 pad_0x0[0x90];
    UnkSluggerMovementData* field_0x90;
};

struct UnkSluggerMovement {
    u8 pad_0x0[0x10];
    UnkReflObject* field_0x10;
};

struct UnkSluggerSpitAttack_PickId {
    u8 pad_0x0[8];
    UnkActor* owner;
};

const CStrId* CSluggerSpitAttack_SelectAcidballInitAnim(UnkSluggerSpitAttack_PickId* self) {
    if (!self->owner)
        return &base::global::CStrId::Empty();
    UnkAnimationComponent* anim = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    UnkSluggerMovement* movement = static_cast<UnkSluggerMovement*>(self->owner->field_0x218);
    UnkReflObject* part = movement ? movement->field_0x10 : nullptr;
    UnkSluggerMovementPart* enemy = nullptr;
    if (part && fn_710009d50c(part->vf_0x0(), fn_710095629c()))
        enemy = reinterpret_cast<UnkSluggerMovementPart*>(part);
    if (!self->owner)
        return &base::global::CStrId::Empty();
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!anim || !enemy || !target)
        return &base::global::CStrId::Empty();
    bool flag = fn_710117ae38(anim, enemy->field_0x90->field_0x30, false);
    UnkActor* actor = self->owner;
    f32 tx = target->field_0x44;
    f32 ty = target->field_0x54;
    f32 tz = target->field_0x64;
    f32 ax = actor->field_0x44;
    f32 ay = actor->field_0x54;
    f32 az = actor->field_0x64;
    f32 dx = tx - ax;
    f32 dy = ty - ay;
    f32 dz = tz - az;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
    f32 fx = m.m[2];
    f32 fy = m.m[6];
    f32 fz = m.m[10];
    f32 dot = dx * fx + dy * fy + dz * fz;
    bool behind = dot < -0.001f;
    if (flag)
        return behind ? &g_unk_7172707bd0[2] : &g_unk_7172707bd0[0];
    return behind ? &g_unk_7172707bd0[3] : &g_unk_7172707bd0[1];
}
