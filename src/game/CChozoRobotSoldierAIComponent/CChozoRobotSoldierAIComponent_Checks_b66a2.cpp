// CChozoRobotSoldierAIComponent unit (tools/tu_map.py, the name from reflection): two checks.
// fn_7100473fdc: after the base check (fn_71011074b8), true unless the actor's target (fn_7100d7c990) has
// a +0x208 object that fn_71009e12d0 accepts. fn_71004743f0: after the base check (fn_710110f3cc, same
// arguments), true unless `id` is in the u16 list at +0x27768 (count +0x27770). Names and layouts are
// placeholders.
#include "game/UnkActor.h"
#include "types.h"

// As src/game/CBatalloonAIComponent/fn_71003de7b4.cpp declares it.
bool fn_71009e12d0(void*);

struct UnkChozoRobotSoldierAI_b66a2 {
    u8 pad_0x0[0x30];
    UnkActor* actor;
    u8 pad_0x38[0x27768 - 0x38];
    u16* ids;
    u32 id_count;
};

bool fn_71011074b8(UnkChozoRobotSoldierAI_b66a2* self);
bool fn_710110f3cc(UnkChozoRobotSoldierAI_b66a2* self, u16 id, u32 a2, void* a3, void* a4, void* a5, u32 a6, u32 a7);

bool fn_7100473fdc(UnkChozoRobotSoldierAI_b66a2* self) {
    if (!fn_71011074b8(self)) {
        return false;
    }
    UnkActor* target = fn_7100d7c990(self->actor);
    if (!target || !target->field_0x208) {
        return true;
    }
    return !fn_71009e12d0(target->field_0x208);
}

// The base check reads w2, x5, w6 and w7 too, which the original passes on untouched (x3 and x4
// with them; tools/forwarded_args.py).
bool fn_71004743f0(UnkChozoRobotSoldierAI_b66a2* self, u16 id, u32 a2, void* a3, void* a4, void* a5, u32 a6, u32 a7) {
    if (!fn_710110f3cc(self, id, a2, a3, a4, a5, a6, a7)) {
        return false;
    }
    for (const u16 *it = self->ids, *end = self->ids + self->id_count; it != end; ++it) {
        if (*it == id) {
            return false;
        }
    }
    return true;
}
