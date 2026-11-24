// A constructor in the CChangeSetupLogicAction unit: the base constructor fn_7100a55aa4, the vtable, an
// empty string id at +0x10, then the flags at +0x18 and +0x1a set (+0x19 cleared). Names and layouts are
// placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/CTriggerLogicAction.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkChangeSetupLogicActionDef {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkStrId field_0x10;
    bool bPersistent;
    bool bForceChange;
    bool bPush;
};

extern void* g_unk_7101a8e380[];

void fn_7100a377a4(UnkChangeSetupLogicActionDef* self) {
    fn_7100a55aa4(self);
    self->vtable = &g_unk_7101a8e380[2];
    new (&self->field_0x10) UnkStrId("", true);
    self->bPersistent = true;
    self->bForceChange = false;
    self->bPush = true;
}
