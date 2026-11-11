// CEnemyMovementWalkingAnims' constructor: vtable g_unk_7101a38c40 + 0x10, then the seven animation ids "relax",
// "walk", "back", "walkinit", "backinit", "walkend" and "backend" (as inlined in
// src/game/CCharClassChozoRobotSoldierAIComponent/fn_710046e130.cpp). Names are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/CCharClassChozoRobotSoldierAIComponent.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkEnemyMovementWalkingAnims_A5_b68 {
    void** vtable;
    UnkStrId field_0x8;
    UnkStrId field_0x10;
    UnkStrId field_0x18;
    UnkStrId field_0x20;
    UnkStrId field_0x28;
    UnkStrId field_0x30;
    UnkStrId field_0x38;
};

void fn_71009689cc(UnkEnemyMovementWalkingAnims_A5_b68* self) {
    self->vtable = &g_unk_7101a38c40[2];
    new (&self->field_0x8) UnkStrId("relax", true);
    new (&self->field_0x10) UnkStrId("walk", true);
    new (&self->field_0x18) UnkStrId("back", true);
    new (&self->field_0x20) UnkStrId("walkinit", true);
    new (&self->field_0x28) UnkStrId("backinit", true);
    new (&self->field_0x30) UnkStrId("walkend", true);
    new (&self->field_0x38) UnkStrId("backend", true);
}
