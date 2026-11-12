// A CYamplotXStepAttack check: the base attack's check (fn_7100360b84) passes, the actor has a
// target (fn_7100d7c990), fn_71006ed714 accepts the actor and the target's object at +0x208 is
// absent or accepted by fn_71009e12d0. Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "game/UnkAttack.h"
#include "game/CAttack.h"

bool fn_71006ed714(UnkAttackBase* self, UnkActor* actor);
struct UnkHelper;
bool fn_71009e12d0(void*);

bool fn_71006ed69c(UnkAttackBase* self, void* a, u32 b) {
    bool ok = fn_7100360b84(reinterpret_cast<CAttack*>(self), a, b);
    UnkActor* target = fn_7100d7c990(self->field_0x8);
    if (ok && target && fn_71006ed714(self, self->field_0x8)) {
        if (!target->field_0x208 || fn_71009e12d0(reinterpret_cast<UnkHelper*>(target->field_0x208)))
            return true;
    }
    return false;
}
