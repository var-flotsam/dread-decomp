// Destructor: resets the vtable, releases the string id at +0x68, then the sub-object at +0x38 and the base.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/behaviortree/CStringEqualsCondition.h"
#include "types.h"

struct UnkObj_3a3b78 {
    void** vtable;
    u8 pad_0x8[0x30];
    base::global::CRntString sRawValue;
    UnkStrIdTwoStep field_0x68;
};

void fn_7100394530(void*);

void CStringEqualsCondition_Destroy(UnkObj_3a3b78* self) {
    self->vtable = &g_unk_7101a28ac0[2];
    self->field_0x68.~UnkStrIdTwoStep();
    CRntString_Destroy(&self->sRawValue);
    fn_7100394530(self);
}
