// CChozoRobotSoldierUppercutAttack (tools/tu_map.py): whether the attack may start, as the dash slash's
// check (src/game/CChozoRobotSoldierDashSlashAttack/CChozoRobotSoldierDashSlashAttack_b49.cpp) with four
// state tests; when the actor it acts against is busy (+0x208, fn_71009e12d0) the answer is the AI
// component's flag at +0x3f. Names and layouts are placeholders.
#include "../CChozoRobotSoldierDashSlashAttack/UnkRobotSoldierAttack_b49.h"

u8 fn_710046a394(UnkDashSlashAttack_b49* self, void* a, void* b) {
    bool ok = reinterpret_cast<bool (*)(UnkDashSlashAttack_b49*, void*, void*)>(fn_7100360b84)(self, a, b);
    if (!ok)
        return false;
    UnkRobotSoldierManager_b49* manager = reinterpret_cast<UnkRobotSoldierManager_b49*>(GetRobotSoldierManager());
    UnkRobotSoldierActor_b49* actor = self->actor;
    if (manager->actor != actor)
        return false;
    if (UnkRobotSoldierAI_b49* ai = self->actor->ai) {
        if (fn_710009d50c(ai->vf_0x0(), fn_710046ea2c())) {
            if (fn_7100472fa4(ai) || fn_71004718fc(ai) || fn_71004744a4(ai) || fn_710110a0a4(ai))
                return false;
        }
    }
    UnkRobotSoldierTarget_b49* target = reinterpret_cast<UnkRobotSoldierTarget_b49*>(
        fn_7100d7c990(reinterpret_cast<UnkActor*>(self->actor)));
    if (target && target->field_0x208 && fn_71009e12d0(target->field_0x208)) {
        if (UnkRobotSoldierAI_b49* ai = self->actor->ai)
            return ai->field_0x3f;
    }
    return true;
}
