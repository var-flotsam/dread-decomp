// CChozoRobotSoldierAIComponent's unit (tools/tu_map.py unit 694): a message handler after the base's
// (fn_71003adaf8). One kind of message turns the owner a quarter turn away from the side it faces; a hit
// from the component's own attack is scaled by the tuning and sends the owner's timeline the type id. Names and layouts are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CBossAIComponent.h"
#include "game/CCharClassChozoRobotSoldierAIComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU1730.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

using base::math::CVector3D;

struct UnkRobotSoldierKey_b61a2;
extern UnkRobotSoldierKey_b61a2 g_unk_71016497e8;  // reached through the GOT

struct UnkRobotSoldierTuning_l471d48 {
    u8 pad_0x0[0x3c];
    f32 field_0x3c;  // a damage factor
};
struct UnkRobotSoldierTuning_ovb0002;  // CChozoRobotSoldierAIComponent_Checks_ovb0002.cpp's view of the tuning

struct UnkRobotSoldierLife_l471d48 {
    u8 pad_0x0[0x54];
    f32 field_0x54;
};

struct UnkRobotSoldierOwner_l471d48 {
    void** vtable;
    u8 pad_0x8[0x1b0];
    UnkRobotSoldierLife_l471d48* field_0x1b8;
    u8 pad_0x1c0[0x58];
    void* field_0x218;
    void* field_0x220;
    u8 pad_0x228[0xc0];
    UnkTimelineComponent* field_0x2e8;
};
static_assert(__builtin_offsetof(UnkRobotSoldierOwner_l471d48, field_0x2e8) == 0x2e8, "layout");

struct UnkAttackSource_l471d48 {
    u8 pad_0x0[0x18];
    void* field_0x18;
};

struct UnkRobotSoldierMessage_l471d48 {
    u8 pad_0x0[0x60];
    UnkAttackSource_l471d48* source;  // 0x60
    u8 pad_0x68[0x4c];
    u32 flags;  // 0xb4
    u8 pad_0xb8[0x40];
    f32 damage;  // 0xf8
};
static_assert(__builtin_offsetof(UnkRobotSoldierMessage_l471d48, damage) == 0xf8, "layout");

struct UnkAttackDef_l471d48 {
    u8 pad_0x0[0x30];
    void* field_0x30;
};

struct UnkAttack_l471d48 {
    u8 pad_0x0[0x150];
    UnkAttackDef_l471d48* def;  // 0x150
};

struct UnkRobotSoldierAI_l471d48 {
    u8 pad_0x0[0x30];
    UnkRobotSoldierOwner_l471d48* owner;
    u8 pad_0x38[0x149a0 - 0x38];
    UnkAttack_l471d48* attack;  // 0x149a0
    u8 pad_0x149a8[0x14b78 - 0x149a8];
    bool hit;  // 0x14b78
};
static_assert(__builtin_offsetof(UnkRobotSoldierAI_l471d48, hit) == 0x14b78, "layout");

u32 fn_7100471d48(UnkRobotSoldierAI_l471d48* self, UnkRobotSoldierMessage_l471d48* msg) {
    self->hit = false;
    u32 result = fn_71003adaf8(reinterpret_cast<UnkQuetzoaXAI_b58*>(self),
                               reinterpret_cast<UnkQuetzoaXMessage_b58*>(msg));
    if (result)
        return result;

    if (msg->flags & 0x8000) {
        UnkRobotSoldierOwner_l471d48* owner = self->owner;
        void* movement = owner->field_0x218;
        void* target = owner->field_0x220;
        UnkActor* actor = fn_7100d7c990(reinterpret_cast<UnkActor*>(owner));
        if (movement && target && actor && fn_71010cc3a4(movement) == 1) {
            base::reflection::UnkReflTypeIdArg value;
            base::reflection::UnkReflTypeIdArg arg;
            CVector3D angles;
            value.id = 1;
            arg.id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
            CAnimationComponent_SendActionToTrack(target, 0x9b214780df6c8f00, 0xf4061ff25a2163d6, &value, &g_unk_710164b958,
                          &g_unk_7172769740, &arg);
            UnkRobotSoldierOwner_l471d48* o = self->owner;
            const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
            f32 y = m.m[2] >= 0.0f ? -1.5707964f : 1.5707964f;
            angles.x = 0.0f;
            angles.z = 0.0f;
            angles.y = y;
            reinterpret_cast<void (*)(UnkRobotSoldierOwner_l471d48*, CVector3D*, bool)>(o->vtable[0xd8 / 8])(
                o, &angles, true);
        }
    } else if (!(msg->flags & 0x280156fe)) {
        return 2;
    }

    UnkRobotSoldierLife_l471d48* life = self->owner->field_0x1b8;
    if (life && life->field_0x54 > 0.0f && msg->source &&
        self->attack->def->field_0x30 == msg->source->field_0x18) {
        fn_71008c1538(reinterpret_cast<UnkRequest*>(msg), msg->damage * reinterpret_cast<UnkRobotSoldierTuning_l471d48&>(g_unk_71726cc6d8).field_0x3c);
        self->hit = true;
        if (UnkTimelineComponent* timeline = self->owner->field_0x2e8) {
            base::reflection::UnkReflTypeIdArg arg;
            arg.id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
            fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_71016497e8), &arg.id, false, -1.0f);
        }
    }
    return 0;
}
