// CChozoRobotSoldierUppercutAttack's unit (tools/tu_map.py unit 687): the attack's update. After the base
// update (fn_71003609a8), once (flag at +0x40) and while neither the attack's track nor g_unk_71726cbe80's is
// playing, it sends the owner's (+0x8) animation one of two ids when the target (fn_7100d7c990) is in reach of
// the CChozoRobotSoldierAIComponent: near (the params' +0xfc) or far. Names and layouts are placeholders.
#include <math.h>

#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2702.h"
#include "game_s2/UnkSpitAttack.h"
#include "types.h"

struct UnkCrsUpHashed_l46a5f4 {
    u8 pad_0x0[0x30];
    u64 hash;
};

extern UnkCrsUpHashed_l46a5f4* g_unk_71726cbe80;  // reached through the GOT

// The attack's parameters: the track it plays and the reach it attacks within.
struct UnkCrsUpParams_l46a5f4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual UnkCrsUpHashed_l46a5f4** vf_0x18(UnkActor* owner);
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual f32 vf_0x30();
    u8 pad_0x8[0xec - 0x8];
    f32 field_0xec;
    f32 field_0xf0;
    f32 field_0xf4;
    f32 field_0xf8;
    f32 field_0xfc;
};
static_assert(__builtin_offsetof(UnkCrsUpParams_l46a5f4, field_0xfc) == 0xfc, "layout");

struct UnkCrsUpAI_l46a5f4 {
    virtual const base::reflection::CType* vf_0x0();
    u8 pad_0x8[0x3f - 0x8];
    bool field_0x3f;
};

struct UnkCrsUpTarget_l46a5f4 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x210 - 0x68];
    void* field_0x210;
};

struct UnkCrsUpAttack_l46a5f4 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x18];
    UnkCrsUpParams_l46a5f4* params;  // 0x28
    f32 field_0x30;
    u8 pad_0x34[0x8];
    f32 field_0x3c;
    bool sent;  // 0x40
};

void fn_710046a5f4(UnkCrsUpAttack_l46a5f4* self, f32 dt) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), dt);
    if (self->sent)
        return;
    UnkAnimationComponent* animation = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (!animation)
        return;
    if (fn_710117ae38(animation, (*self->params->vf_0x18(self->owner))->hash, false))
        return;
    if (fn_710117ae38(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), g_unk_71726cbe80->hash, false))
        return;
    auto* target = reinterpret_cast<UnkCrsUpTarget_l46a5f4*>(fn_7100d7c990(self->owner));
    auto* ai = static_cast<UnkCrsUpAI_l46a5f4*>(self->owner->field_0x240);
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_710046ea2c()))
        return;
    if (!target || !self->owner->field_0x220)
        return;

    bool in_range;
    if (self->params->vf_0x30() > 0.0f &&
        fabsf(self->field_0x30 - self->owner->field_0x44) >= self->params->vf_0x30() + self->params->field_0xec) {
        in_range = true;
    } else {
        f32 height = 0.0f;
        fn_71010fc68c(reinterpret_cast<UnkGoliathAI_b58a3*>(ai), &height);
        in_range = height <= self->field_0x3c + 120.0f;
    }

    f32 distance = fabsf(target->x - self->owner->field_0x44);
    if (!in_range) {
        if (ai->field_0x3f) {
            if (target->field_0x210) {
                if (!(distance < self->params->field_0xf4))
                    return;
            } else if (!(distance < self->params->field_0xf0)) {
                return;
            }
        } else {
            const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
            UnkActor* owner = self->owner;
            f32 dx = target->x - owner->field_0x44;
            f32 dy = target->y - owner->field_0x54;
            f32 dz = target->z - owner->field_0x64;
            if (m.m[2] * dx + m.m[6] * dy + m.m[10] * dz < 0.0f) {
                if (distance <= self->params->field_0xf8)
                    return;
            } else if (!(distance < self->params->field_0xf0)) {
                return;
            }
        }
    }

    void* animation_target = self->owner->field_0x220;
    if (distance < self->params->field_0xfc)
        base::reflection::refl_EmitU32(animation_target, 0xde6f5a5d963ca497, 0xf4061ff25a2163d6, 0,
                                       &g_unk_710164b958, &g_unk_7172769740);
    else
        base::reflection::refl_EmitU32(animation_target, 0x98c6fdc80e86d316, 0xf4061ff25a2163d6, 0,
                                       &g_unk_710164b958, &g_unk_7172769740);
    self->sent = true;
}
