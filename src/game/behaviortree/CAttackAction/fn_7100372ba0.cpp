// Destructor: resets the vtable, releases the string id at +0x60, then the sub-object at +0x30 and the base.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/behaviortree/CAttackAction.h"
#include "game/behaviortree/CBaseAction.h"
#include "types.h"

struct UnkObj_372ba0 {
    void** vtable;
    u8 pad_0x8[0x28];
    base::global::CRntString sAttack;
    UnkStrIdTwoStep field_0x60;
};

void CAttackAction_Destroy(UnkObj_372ba0* self) {
    self->vtable = &g_unk_7101a25010[2];
    self->field_0x60.~UnkStrIdTwoStep();
    CRntString_Destroy(&self->sAttack);
    fn_7100393ea0(self);
}
