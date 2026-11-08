// CChozoRobotSoldierDashSlashAttack (tools/tu_map.py): whether the attack may start. The base check
// (fn_7100360b84) must pass and the attack's actor must be the one the lazily created manager
// (g_unk_71726af468, +0x2a0) holds; a CChozoRobotSoldierAIComponent (+0x240) must pass five state tests
// (fn_7100471278 only matters while fn_7101136308 holds for +0x258); then the actor it acts against
// (fn_7100d7c990) must not be busy (+0x208, fn_71009e12d0) and, when its +0x1b8 is accepted by
// fn_7101161088, its +0x208's +0x2d08 state must not be 9. Names and layouts are placeholders.
#include "UnkRobotSoldierAttack_b49.h"
#include "game/CAttack.h"

bool fn_71004682a8(UnkDashSlashAttack_b49* self, void* a, u32 b) {
    bool ok = fn_7100360b84(reinterpret_cast<CAttack*>(self), a, b);
    if (!ok)
        return false;
    UnkRobotSoldierManager_b49* manager = reinterpret_cast<UnkRobotSoldierManager_b49*>(GetRobotSoldierManager());
    UnkRobotSoldierActor_b49* actor = self->actor;
    if (manager->actor != actor)
        return false;
    if (UnkRobotSoldierAI_b49* ai = self->actor->ai) {
        if (fn_710009d50c(ai->vf_0x0(), fn_710046ea2c())) {
            if (fn_7100472fa4(ai) || fn_71004718fc(ai))
                return false;
            if (!fn_7101136308(self->actor->field_0x258) && !fn_7100471278(ai))
                return false;
            if (fn_71004744a4(ai) || fn_710110a0a4(ai))
                return false;
        }
    }
    UnkRobotSoldierTarget_b49* target = reinterpret_cast<UnkRobotSoldierTarget_b49*>(
        fn_7100d7c990(reinterpret_cast<UnkActor*>(self->actor)));
    if (!target)
        return true;
    if (target->field_0x208 && fn_71009e12d0(target->field_0x208))
        return false;
    if (target->field_0x1b8 && fn_7101161088(target->field_0x1b8) && target->field_0x208 &&
        target->field_0x208->state)
        return target->field_0x208->state->state != 9;
    return true;
}
