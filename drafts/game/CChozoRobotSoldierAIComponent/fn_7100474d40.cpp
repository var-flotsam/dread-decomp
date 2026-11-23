// CChozoRobotSoldierAIComponent's unit (tools/tu_map.py): the base answer (fn_710110a6bc); when it is 1
// and the actor the owner acts against is busy (+0x208, fn_71009e12d0) and the manager's node set does
// not take the owner (fn_7100475aac), 2 when the owner is the manager's attacking actor (+0x2a0), else 1.
// Names and layouts are placeholders.
#include "../../../src/game/CChozoRobotSoldierDashSlashAttack/UnkRobotSoldierAttack_b49.h"
#include "game/UnkActor.h"
#include "game/UnkTU1501.h"
#include "types.h"

struct UnkNodeSet695_b43a1;

bool fn_7100475aac(UnkNodeSet695_b43a1* self, u64 v);
s32 fn_710110a6bc(void* self);

struct UnkRobotSoldierManager_l474d40 {
    u8 pad_0x0[0x228];
    u8 node_set[0x78];  // 0x228
    UnkActor* actor;    // 0x2a0
};

struct UnkRobotSoldierAI_l474d40 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

static inline UnkRobotSoldierManager_l474d40* Manager_l474d40() {
    return reinterpret_cast<UnkRobotSoldierManager_l474d40*>(GetRobotSoldierManager());
}

s32 fn_7100474d40(UnkRobotSoldierAI_l474d40* self) {
    s32 result = fn_710110a6bc(self);
    UnkActor* target;
    if (result == 1 && (target = fn_7100d7c990(self->owner)) && target->field_0x208 &&
        fn_71009e12d0(target->field_0x208) &&
        !fn_7100475aac(reinterpret_cast<UnkNodeSet695_b43a1*>(Manager_l474d40()->node_set),
                       reinterpret_cast<u64>(self->owner)))
        return Manager_l474d40()->actor == self->owner ? 2 : 1;
    return result;
}
