// CShakernautDoubleGroundShockAttack's unit: whether the attack can start: the base attack's check
// (fn_7100360b84), except that when the actor's target has an object at +0x208 that fn_71009e12d0
// accepts, it cannot. Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "game/UnkAttack.h"
#include "game/CAttack.h"
#include "game/UnkTU1501.h"

bool fn_71006a43c4(UnkAttackBase* self, void* a, u32 b) {
    bool ok = fn_7100360b84(reinterpret_cast<CAttack*>(self), a, b);
    if (ok) {
        UnkActor* target = fn_7100d7c990(self->field_0x8);
        if (target && target->field_0x208)
            ok &= !fn_71009e12d0(target->field_0x208);
    }
    return ok;
}
