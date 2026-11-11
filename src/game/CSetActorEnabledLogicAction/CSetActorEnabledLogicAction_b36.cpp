// A constructor in the CSetActorEnabledLogicAction unit: the base constructor fn_7100a55aa4, the
// vtable, the container at +0x10 (base vtable, empty array with step 10 and flags 0xf, the default
// allocator, final vtable), then a cleared word and flag after it. Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "game/CTriggerLogicAction.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkChangeSetupLogicActionDef;
extern void* g_unk_7101a8f3d8[];

struct UnkSetActorEnabledLogicActionDef {
    void** vtable;
    u8 pad_0x8[0x8];
    void** field_0x10;
    u64 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    bool field_0x2c;
    u32 field_0x30;
    base::UnkAllocator* field_0x38;
    u32 field_0x40;
    u64 field_0x48;
    bool field_0x50;
};

void fn_7100a51ea0(UnkSetActorEnabledLogicActionDef* self) {
    fn_7100a55aa4(reinterpret_cast<UnkChangeSetupLogicActionDef*>(self));
    self->vtable = g_unk_7101a8f3d8 + 2;
    self->field_0x10 = g_unk_7101a20998 + 2;
    self->field_0x18 = 0;
    self->field_0x20 = 0;
    self->field_0x28 = 10;
    self->field_0x2c = true;
    self->field_0x30 = 0xf;
    self->field_0x38 = fn_7100083990();
    self->field_0x40 = 0;
    self->field_0x10 = g_unk_7101a20950 + 2;
    self->field_0x48 = 0;
    self->field_0x50 = false;
}
