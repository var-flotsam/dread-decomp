// A constructor in the CPerceptionModifierLogicAction unit: the base constructor fn_7100a55aa4, the
// vtable, a cleared word at +0x10, the container at +0x18 (base vtable, empty array with step 10
// and flags 0xf, the default allocator, final vtable), then cleared fields after it. Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkChangeSetupLogicActionDef;
void fn_7100a55aa4(UnkChangeSetupLogicActionDef* self);  // the logic-action base constructor
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a20950[];
extern void* g_unk_7101a8f398[];

struct UnkPerceptionModifierLogicActionDef {
    void** vtable;
    u8 pad_0x8[0x8];
    u32 field_0x10;
    void** field_0x18;
    u64 field_0x20;
    u64 field_0x28;
    u32 field_0x30;
    bool field_0x34;
    u32 field_0x38;
    base::UnkAllocator* field_0x40;
    u32 field_0x48;
    u64 field_0x50;
    u32 field_0x58;
};

void fn_7100a515b4(UnkPerceptionModifierLogicActionDef* self) {
    fn_7100a55aa4(reinterpret_cast<UnkChangeSetupLogicActionDef*>(self));
    self->vtable = g_unk_7101a8f398 + 2;
    self->field_0x10 = 0;
    self->field_0x18 = g_unk_7101a20998 + 2;
    self->field_0x20 = 0;
    self->field_0x28 = 0;
    self->field_0x30 = 10;
    self->field_0x34 = true;
    self->field_0x38 = 0xf;
    self->field_0x40 = fn_7100083990();
    self->field_0x48 = 0;
    self->field_0x18 = g_unk_7101a20950 + 2;
    self->field_0x50 = 0;
    self->field_0x58 = 0;
}
