// A constructor in the CChangeSetupLogicAction unit: the base constructor fn_7100a55aa4, the vtable, an
// empty string id at +0x10, then the flags at +0x18 and +0x1a set (+0x19 cleared). Names and layouts are
// placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkChangeSetupLogicActionDef {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkStrId field_0x10;
    bool field_0x18;
    bool field_0x19;
    bool field_0x1a;
};

void fn_7100a55aa4(UnkChangeSetupLogicActionDef* self);
extern void* g_unk_7101a8e380[];

void fn_7100a377a4(UnkChangeSetupLogicActionDef* self) {
    fn_7100a55aa4(self);
    self->vtable = &g_unk_7101a8e380[2];
    new (&self->field_0x10) UnkStrId("", true);
    self->field_0x18 = true;
    self->field_0x19 = false;
    self->field_0x1a = true;
}
