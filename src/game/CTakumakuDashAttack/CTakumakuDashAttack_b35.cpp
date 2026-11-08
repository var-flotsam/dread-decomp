// A CTakumakuDashAttack check: the base attack's check (fn_7100360b84) passes and the actor has a target
// (fn_7100d7c990; the lookup runs whatever the base check says). Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "game/UnkAttack.h"
#include "game/CAttack.h"


bool fn_71006ce7d4(UnkAttackBase* self, void* a, u32 b) {
    bool ok = fn_7100360b84(reinterpret_cast<CAttack*>(self), a, b);
    UnkActor* target = self->field_0x8 ? fn_7100d7c990(self->field_0x8) : nullptr;
    return ok && target;
}
