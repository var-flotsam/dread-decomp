// behaviortree::CChozoRobotSoldierPatrolAction's unit (tools/tu_map.py unit 689): fn_710046bba0 starts the
// action, and when the owner's AI is a CChozoRobotSoldierAIComponent with navigation and a patrol target, hands
// the navigation a kind-7 parameter block aimed at the target's path. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAIComponent.h"
#include "game/UnkTU365.h"
#include "game/behaviortree/CBehavior.h"
#include "game_s2/UnkParams7.h"
#include "types.h"
#include "game/behaviortree/CGoToWallFronPathAction.h"

// As src/game/CMagnetMovablePlatformComponent/CMagnetMovablePlatformComponent_Point_b52.cpp declares it.
struct UnkMagnetPathHolder_b52;

// What fn_7100382150 finds for the target actor: its path starts at +0x30.
struct UnkPatrolPathHolder_l46bba0 {
    u8 pad_0x0[0x30];
    u8 path[1];  // 0x30
};

// The patrol target handle the AI's part (+0x14ad8) resolves to.
struct UnkPatrolTarget_l46bba0 {
    u8 pad_0x0[0x10];
    UnkActor* actor;  // 0x10
};

struct UnkChozoRobotSoldierAI_l46bba0 {
    void** vtable;
    u8 pad_0x8[0x14ad8 - 0x8];
    u8 part[1];  // 0x14ad8

    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkChozoRobotSoldierAI_l46bba0*)>(vtable[0])(
            this);
    }
};

struct UnkPatrolOwner_l46bba0 {
    u8 pad_0x0[0x240];
    UnkChozoRobotSoldierAI_l46bba0* ai;  // 0x240
};

struct UnkChozoRobotSoldierPatrolAction_l46bba0 {
    u8 pad_0x0[0x18];
    UnkPatrolOwner_l46bba0* owner;  // 0x18
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_710046bba0(UnkChozoRobotSoldierPatrolAction_l46bba0* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    UnkPatrolOwner_l46bba0* owner = self->owner;
    if (!owner)
        return;
    UnkChozoRobotSoldierAI_l46bba0* ai = owner->ai;
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_710046ea2c()))
        return;
    UnkNavigation* navigation = fn_71010fccf8(reinterpret_cast<UnkAIComponent*>(ai));
    if (!navigation)
        return;
    UnkPatrolTarget_l46bba0* target = reinterpret_cast<UnkPatrolTarget_l46bba0*>(
        *fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(ai->part)));
    if (!target || !target->actor)
        return;
    UnkPatrolPathHolder_l46bba0* holder =
        reinterpret_cast<UnkPatrolPathHolder_l46bba0*>(fn_7100382150(target->actor));

    UnkParams7 params;  // field_0x38 is unset in the original
    __builtin_memset(params.field_0x8, 0, sizeof(params.field_0x8));
    params.field_0x1c = 0;
    __builtin_memset(params.field_0x20, 0, sizeof(params.field_0x20));
    params.field_0x30 = 0;
    params.vtable = &g_unk_7101a265a8[2];
    params.field_0x40 = 0;
    params.field_0x50 = 0;
    params.field_0x58 = 0;
    params.field_0x5a = 0;
    params.field_0x5c = 0;
    params.field_0x5e = 0;
    params.field_0x64 = 0;
    params.field_0x98 = 0;
    params.field_0x78 = 0;
    params.field_0x90 = 0;
    params.field_0x34 = 7;
    __builtin_memset(params.field_0x68, 0, sizeof(params.field_0x68));
    __builtin_memset(params.field_0x80, 0, sizeof(params.field_0x80));
    params.field_0x18 = 1.0f;
    params.field_0x59 = 1;
    params.field_0x5a = 1;
    params.field_0x5d = 1;
    params.field_0x5e = 1;
    params.field_0x1c = 1;
    params.field_0x48 = holder->path;
    params.field_0x50 = 1;
    params.field_0x54 = 3;
    params.field_0x58 = 1;
    fn_71006f4b0c(navigation, &params, 1);
}
